#include "WindowedApplication.h"

#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"

#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Device.h"
#include "vulkan/core/RenderPass.h"
#include "vulkan/core/PipelineLayout.h"
#include "vulkan/core/Pipeline.h"

#include "common/fileio.h"
#include <sstream>

WindowedApplication::WindowedApplication() :
    m_windowProperties()
{ }

WindowedApplication::WindowedApplication(const Window::Properties& properties) :
    m_windowProperties(properties)
{ }

WindowedApplication::~WindowedApplication()
{ 
    // Not destroying in correct order but this isn't meant to be permenant
    m_vkDevice->wait_idle();
}

ExitFlags WindowedApplication::app_main()
{
    if( !create_window() )
    {
        return ExitFlagBits::InitFailure;
    }

    if( !create_render_context() )
    {
        return ExitFlagBits::InitFailure;
    }

    create_vulkan_things();

    float clearC = 1.f;

    while( !m_window->get_should_close() )
    {
        static double time;
        double newTime = glfwGetTime();
        double deltaTime = (time == 0.0) ? (1.0 / 60.0) : newTime - time;
        time = newTime;

        m_window->process_events();
        
        vk::CommandBuffer& commandbuffer = m_renderContext->begin(vk::CommandBuffer::ResetMode::AlwaysAllocate);

        vk::RenderTarget& cRenderTarget = m_renderContext->get_active_frame().get_render_target();
        vk::Framebuffer& cFramebuffer = m_vkDevice->get_resource_cache().request_framebuffer(cRenderTarget, *m_vulkanThings.renderPass);
        const auto& cViews = cRenderTarget.get_image_views();
        commandbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr, &cFramebuffer, 0);

        commandbuffer.get_pipeline_state().set_pipeline_layout(*m_vulkanThings.pipelineLayout);
        commandbuffer.get_pipeline_state().set_render_pass(*m_vulkanThings.renderPass);

        vk::ColorBlendState colorstate;
        colorstate.attachments.push_back(vk::ColorBlendAttachmentState());
        commandbuffer.get_pipeline_state().set_color_blend_state(colorstate);

        VkClearValue clear{ };
        clear.color = { 0.2f, 0.2f, 0.2f, 1.f };
        VkClearValue clear2{ };
        clear2.depthStencil = { 1.f, 0 };

        commandbuffer.begin_render_pass(&cRenderTarget, *m_vulkanThings.renderPass, cFramebuffer, { clear, clear2 });
        vkCmdBindPipeline(commandbuffer.get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_vulkanThings.pipeline->get_handle());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = 1200.f;
        viewport.height = 900.f;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandbuffer.get_handle(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = { 1200, 900 };
        vkCmdSetScissor(commandbuffer.get_handle(), 0, 1, &scissor);

        vkCmdDraw(commandbuffer.get_handle(), 3, 1, 0, 0);

        commandbuffer.end_render_pass();
        commandbuffer.end();

        m_renderContext->submit_and_end(commandbuffer);

        if( clearC == 1.f )
            clearC = 0.f;
        else
            clearC = 1.f;

        std::stringstream ss;
        ss << "FPS: " << std::to_string(static_cast<int>(1.f / deltaTime)) << " | Frametime: " << deltaTime * 1000 << "ms";
        m_window->set_title(ss.str());
    }

    return ExitFlagBits::Success;
}

void WindowedApplication::on_event(Event& e)
{
    get_log().event(__FUNCTION__, "{}", e.to_str());
}

vk::RenderContext& WindowedApplication::get_render_context()
{
    return *m_renderContext;
}

Window& WindowedApplication::get_window()
{
    return *m_window;
}

bool WindowedApplication::create_window()
{
    m_windowProperties.eventfn = BIND_EVENT_FN(WindowedApplication::on_event);
    return create_window(m_windowProperties);
}

bool WindowedApplication::create_window(Window::Properties& properties)
{
    try
    {
        m_window = std::make_unique<WindowGlfw>(get_log(), properties);
    }
    catch( std::exception e )
    {
        JCLOG_EXCEPTION(get_log(), e, "Failed to create GLFW window.");
        return false;
    }

    return true;
}

bool WindowedApplication::create_render_context()
{
    std::vector<const char*> requestedInstanceExtensions = request_instance_extensions();
    std::vector<const char*> requestedValidationLayers;
    std::vector<const char*> requestedDeviceExtensions = request_device_extensions();
    requestedDeviceExtensions.push_back("VK_KHR_swapchain");

#ifdef CFG_DEBUG
    requestedInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    requestedValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
    for( const char* requestedLayer : request_validation_layers() )
    {
        requestedValidationLayers.push_back(requestedLayer);
    }
#endif
    
    for( const char* requiredExt : m_window->get_required_surface_extensions() )
    {
        requestedInstanceExtensions.push_back(requiredExt);
    }

    try
    {
        m_vkInstance = std::make_unique<vk::Instance>(
            get_log(),
            m_window->get_properties().title,
            "JCVE",
            VK_API_VERSION_1_3,
            requestedInstanceExtensions,
            requestedValidationLayers);

        m_vkDevice = std::make_unique<vk::Device>(
            get_log(),
            m_vkInstance->get_first_gpu(),
            m_window->create_surface(*m_vkInstance),
            requestedDeviceExtensions);

        m_renderContext = std::make_unique<vk::RenderContext>(
            *m_vkDevice,
            m_vkDevice->get_surface(),
            *m_window,
            request_swapchain_present_mode(),
            request_swapchain_format());
    }
    catch( VulkanException e )
    {
        JCLOG_EXCEPTION(get_log(), e, "Failed to initialize vulkan.");
        return false;
    }

    return true;
}

std::vector<const char*> WindowedApplication::request_instance_extensions() const
{
    return { };
}

std::vector<const char*> WindowedApplication::request_device_extensions() const
{
    return { };
}

std::vector<const char*> WindowedApplication::request_validation_layers() const
{
    return { };
}

std::vector<VkPresentModeKHR> WindowedApplication::request_swapchain_present_mode() const
{
    return { VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR };
}

std::vector<VkSurfaceFormatKHR> WindowedApplication::request_swapchain_format() const
{
    return { { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR } };
}

void WindowedApplication::create_vulkan_things()
{
    std::vector<vk::Attachment> attachments({
        { VK_FORMAT_R8G8B8A8_UNORM,  VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT },
        { VK_FORMAT_D32_SFLOAT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT }});
    std::vector<vk::LoadStoreInfo> infos({
        { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE },
        { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE }});
    std::vector<vk::SubpassInfo> subpassInfos({ { 
        { },
        { 0 },
        { },
        false,
        1,
        VK_RESOLVE_MODE_NONE,
        "Subpass" } });

    m_vulkanThings.renderPass = std::make_unique<vk::RenderPass>(*m_vkDevice, attachments, infos, subpassInfos);

    std::string vertPath = "shaders/triangle.vert";
    std::vector<char> vertSource = FileIO::try_read_file(vertPath).value();
    std::vector<uint8_t> vertSrc(vertSource.size());
    memcpy(vertSrc.data(), vertSource.data(), vertSrc.size());

    std::string fragPath = "shaders/triangle.frag";
    std::vector<char> fragSource = FileIO::try_read_file(fragPath).value();
    std::vector<uint8_t> fragSrc(fragSource.size());
    memcpy(fragSrc.data(), fragSource.data(), fragSrc.size());

    vk::ShaderModule& vertModule = m_vkDevice->get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, vertSrc, "main");
    vk::ShaderModule& fragModule = m_vkDevice->get_resource_cache().request_shader_module(VK_SHADER_STAGE_FRAGMENT_BIT, fragSrc, "main");

    std::vector<vk::ShaderModule*> modules({ &vertModule, &fragModule });

    m_vulkanThings.pipelineLayout = std::make_unique<vk::PipelineLayout>(*m_vkDevice, modules);
    m_vulkanThings.pipelineState.set_pipeline_layout(*m_vulkanThings.pipelineLayout);
    m_vulkanThings.pipelineState.set_render_pass(*m_vulkanThings.renderPass);

    vk::ColorBlendState colorstate;
    colorstate.attachments.push_back(vk::ColorBlendAttachmentState());
    m_vulkanThings.pipelineState.set_color_blend_state(colorstate);

    m_vulkanThings.pipeline = std::make_unique<vk::GraphicsPipeline>(*m_vkDevice, VK_NULL_HANDLE, m_vulkanThings.pipelineState);
}