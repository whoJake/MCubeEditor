#include "WindowedApplication.h"

#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"

#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Device.h"

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

    if( !create_render_context() )
    {
        return ExitFlagBits::InitFailure;
    }

    while( !m_window->get_should_close() )
    {
        m_window->process_events();
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
