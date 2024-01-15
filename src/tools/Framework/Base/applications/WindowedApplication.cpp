#include "WindowedApplication.h"

#include "Windows.h"
#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"
#include <cstdlib>

#include <sstream>

#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Device.h"

WindowedApplication::WindowedApplication()
{ }

WindowedApplication::~WindowedApplication()
{ }

ExitFlags WindowedApplication::app_main()
{
    get_environment().import_var("target", "x:/project/build/dev");
    get_environment().import_var("target_ps5", "$(target)/ps5");
    get_environment().import_var("target_ps5", "$(target)/ps5/level");

    std::string subst = get_environment().subst("$(target)/xbsx");
    std::string subst2 = get_environment().subst("$(target_ps5)/bvh");

    get_log().info("app", "{}", subst);
    get_log().info("app", "{}", subst2);

    // Create log
    for( int i = 0; i < 10; i++ )
    {
        float zeroOne = static_cast<float>(std::rand()) / RAND_MAX;
        jclog::Level lev = (jclog::Level)(8 * zeroOne);

        float rand2 = static_cast<float>(std::rand()) / RAND_MAX;
        std::string logstr = std::format("{}", rand2);
        get_log().log(lev, "app", logstr.c_str());
    }

    Window::Properties winProperties;
    winProperties.mode = Window::Mode::Windowed;
    winProperties.extent = { 500, 500 };
    winProperties.resizable = true;
    winProperties.vsync = Window::VSync::Default;
    winProperties.eventfn = BIND_EVENT_FN(WindowedApplication::on_event);

    Window* glfw = new WindowGlfw(get_log(), winProperties);
    glfw->set_title("This is the title.");

    std::vector<const char*> vkInstanceExtensions = glfw->get_required_surface_extensions();
    vkInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    vk::Instance vkInstance(get_log(),
        "MCubeEditor",
        "vk",
        VK_API_VERSION_1_3,
        vkInstanceExtensions,
        { "VK_LAYER_KHRONOS_validation" });

    VkSurfaceKHR vkSurface = glfw->create_surface(vkInstance);

    vk::Device vkDevice(get_log(),
        vkInstance.get_first_gpu(),
        vkSurface);

    while( !glfw->get_should_close() )
    {
        glfw->process_events();
    }

    vkDestroySurfaceKHR(vkInstance.get_handle(), vkSurface, nullptr);
    delete glfw;

    return ExitFlagBits::Success;
}

void WindowedApplication::on_event(Event& e)
{
    get_log().event(__FUNCTION__, "{}", e.to_str());
}