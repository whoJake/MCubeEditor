#include "WindowGlfw.h"

// Really dirty but can't seem to get fnc binding to work since glfw requires a free function
static jclog::Log* internal_window_log = nullptr;

static void error_callback(int error, const char* msg)
{
    if( internal_window_log )
    {
        internal_window_log->error("GLFW", "GLFW error code {}. {}", error, msg);
    }
}

WindowGlfw::WindowGlfw(jclog::Log& log, const Properties& properties):
    Window(log, properties),
    m_handle(nullptr)
{
    if( !glfwInit() )
        throw std::exception("Failed to initiate GLFW instance");

    internal_window_log = &m_log;
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, properties.resizable);

    GLFWmonitor* monitor = m_properties.mode == Mode::Windowed ? nullptr : glfwGetPrimaryMonitor();
    m_handle = glfwCreateWindow(m_properties.extent.width, m_properties.extent.height, m_properties.title.c_str(), monitor, nullptr);
    set_mode(properties.mode);

    glfwSetWindowUserPointer(m_handle, &m_properties);
    setup_events();
}

WindowGlfw::~WindowGlfw()
{
    internal_window_log = nullptr;
    glfwTerminate();
}

void* WindowGlfw::get_native_handle() const
{
    return m_handle;
}

bool WindowGlfw::get_should_close() const
{
    return glfwWindowShouldClose(m_handle);
}

void WindowGlfw::close()
{
    glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
}

void WindowGlfw::process_events()
{
    glfwPollEvents();
}

void WindowGlfw::set_title(const std::string& title)
{
    glfwSetWindowTitle(m_handle, title.c_str());
}

Window::Extent WindowGlfw::resize(const Window::Extent& extent)
{
    glfwSetWindowSize(m_handle, extent.width, extent.height);
    return extent;
}

void WindowGlfw::set_mode(const Window::Mode& mode)
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    int mX, mY, mWidth, mHeight;
    glfwGetMonitorWorkarea(monitor, &mX, &mY, &mWidth, &mHeight);
    int tlCentreX = mX + ((mWidth - m_properties.extent.width) / 2);
    int tlCentreY = mY + ((mHeight - m_properties.extent.height) / 2);

    switch( mode ) {
        case Mode::Windowed:
        {
            glfwSetWindowAttrib(m_handle, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowMonitor(m_handle, nullptr, tlCentreX, tlCentreY, m_properties.extent.width, m_properties.extent.height, GLFW_DONT_CARE);
            break;
        }
        case Mode::Fullscreen:
        {
            glfwSetWindowMonitor(m_handle, monitor, 0, 0, mWidth, mHeight, GLFW_DONT_CARE);
            break;
        }
        case Mode::FullscreenBorderless:
        {
            set_mode(Mode::Windowed);
            glfwSetWindowAttrib(m_handle, GLFW_DECORATED, GLFW_FALSE);

            const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_handle, nullptr, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
            break;
        }
        case Mode::FullscreenStretched:
        {
            glfwSetWindowMonitor(m_handle, monitor, 0, 0, m_properties.extent.width, m_properties.extent.height, GLFW_DONT_CARE);
            break;
        }
        case Mode::Default:
        {
            set_mode(Mode::Windowed);
            break;
        }
    }
}

void WindowGlfw::setup_events() const
{

}
