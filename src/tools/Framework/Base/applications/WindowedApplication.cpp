#include "WindowedApplication.h"

#include "Windows.h"
#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"
#include <cstdlib>

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
    for( int i = 0; i < 50; i++ )
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

    Window* glfw = new WindowGlfw(get_log(), winProperties);
    glfw->set_title("This is the title.");

    while( !glfw->get_should_close() )
    {
        glfw->process_events();
    }

    delete glfw;

    return ExitFlagBits::Success;
}