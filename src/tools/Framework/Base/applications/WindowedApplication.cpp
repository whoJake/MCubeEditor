#include "WindowedApplication.h"

#include "Windows.h"
#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"
#include <cstdlib>

#include <sstream>

#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Device.h"
#include "vulkan/core/ShaderModule.h"
#include "vulkan/core/DescriptorSetLayout.h"
#include "vulkan/core/DescriptorPool.h"
#include "vulkan/core/PipelineLayout.h"
#include "vulkan/core/DescriptorSet.h"

#include "common/fileio.h"

WindowedApplication::WindowedApplication() :
    m_windowProperties()
{ }

WindowedApplication::WindowedApplication(const Window::Properties& properties) :
    m_windowProperties(properties)
{ }

WindowedApplication::~WindowedApplication()
{ }

ExitFlags WindowedApplication::app_main()
{
    if( !create_window() )
    {
        return ExitFlagBits::InitFailure;
    }

    std::vector<const char*> vkInstanceExtensions = m_window->get_required_surface_extensions();
    vkInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    vk::Instance vkInstance(get_log(),
        "MCubeEditor",
        "vk",
        VK_API_VERSION_1_3,
        vkInstanceExtensions,
        { "VK_LAYER_KHRONOS_validation" });

    VkSurfaceKHR vkSurface = m_window->create_surface(vkInstance);

    vk::Device vkDevice(get_log(),
        vkInstance.get_first_gpu(),
        vkSurface);

    JCLOG_INFO(get_log(), "{} limits.", vkDevice.get_gpu().get_properties().limits.timestampPeriod);

    {
        std::string shaderPath = "shaders/Atomics.frag";
        std::vector<char> sourceBytes = FileIO::try_read_file(shaderPath).value();
        std::vector<uint8_t> source(sourceBytes.size());
        memcpy(source.data(), sourceBytes.data(), source.size());

        vk::ShaderModule& shaderModule = vkDevice.get_resource_cache().request_shader_module(VK_SHADER_STAGE_FRAGMENT_BIT, source, "main");

        std::string shaderPath2 = "shaders/texture3d.vert";
        std::vector<char> sourceBytes2 = FileIO::try_read_file(shaderPath2).value();
        std::vector<uint8_t> source2(sourceBytes2.size());
        memcpy(source2.data(), sourceBytes2.data(), source2.size());

        vk::ShaderModule& shaderModule2 = vkDevice.get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, source2, "main");

        std::vector<vk::ShaderModule*> shaderModuleVec;
        shaderModuleVec.push_back(&shaderModule);
        shaderModuleVec.push_back(&shaderModule2);

        vk::PipelineLayout pipelineLayout(vkDevice, shaderModuleVec);
    }

    while( !m_window->get_should_close() )
    {
        m_window->process_events();
    }

    vkDestroySurfaceKHR(vkInstance.get_handle(), vkSurface, nullptr);

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
    return true;
}
