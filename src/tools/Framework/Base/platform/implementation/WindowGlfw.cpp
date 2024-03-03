#include "WindowGlfw.h"
#include "platform/events/WindowEvent.h"
#include "platform/events/MouseEvent.h"
#include "platform/events/KeyEvent.h"

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

VkSurfaceKHR WindowGlfw::create_surface(vk::Instance& instance)
{
    VkSurfaceKHR surface{ VK_NULL_HANDLE };
    VkResult result = glfwCreateWindowSurface(instance.get_handle(), m_handle, nullptr, &surface);
    VK_CHECK(result, "Failed to create VkSurface on GLFW window.");

    return surface;
}

std::vector<const char*> WindowGlfw::get_required_surface_extensions() const
{
    const char** pExtensions;
    uint32_t extensionCount{ 0 };
    pExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    std::vector<const char*> extensions(pExtensions, pExtensions + extensionCount);
    return extensions;
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

Window::Position WindowGlfw::set_position(const Window::Position& position)
{
    glfwSetWindowPos(m_handle, position.x, position.y);
    int* x = nullptr;
    int* y = nullptr;
    glfwGetWindowPos(m_handle, x, y);

    if( x && y )
    {
        m_properties.position.x = *x;
        m_properties.position.y = *y;
    }

    return m_properties.position;
}

Window::Extent WindowGlfw::set_size(const Window::Extent& extent)
{
    glfwSetWindowSize(m_handle, extent.width, extent.height);
    int* x = nullptr;
    int* y = nullptr;
    glfwGetWindowSize(m_handle, x, y);

    if( x && y )
    {
        m_properties.extent.width = *x;
        m_properties.extent.height = *y;
    }

    return m_properties.extent;
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

void WindowGlfw::set_cursor_lock_state(CursorLockState state)
{
    switch( state )
    {
    case CursorLockState::NONE:
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    case CursorLockState::LOCKED:
        glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if( glfwRawMouseMotionSupported() )
        {
            glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
        break;
    }
}

CursorLockState WindowGlfw::get_cursor_lock_state() const
{
    switch( glfwGetInputMode(m_handle, GLFW_CURSOR) )
    {
    case GLFW_CURSOR_NORMAL:
        return CursorLockState::NONE;
    case GLFW_CURSOR_DISABLED:
        return CursorLockState::LOCKED;
    default:
        return CursorLockState::NONE;
    }
}

double WindowGlfw::poll_mouse_pos_x() const
{
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return x;
}

double WindowGlfw::poll_mouse_pos_y() const
{
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return y;
}

void WindowGlfw::setup_events() const
{
    // Window Events
    {
        glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* owner){
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            WindowClosedEvent e;
            data.eventfn(e);
        });

        glfwSetWindowPosCallback(m_handle, [](GLFWwindow* owner, int x, int y){ 
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            data.position.x = x;
            data.position.y = y;

            WindowMovedEvent e(x, y);
            data.eventfn(e);
        });

        glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* owner, int width, int height) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            data.extent.width = width;
            data.extent.height = height;

            WindowResizeEvent e(width, height);
            data.eventfn(e);
        });

        glfwSetWindowFocusCallback(m_handle, [](GLFWwindow* owner, int focused) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            if( focused ) {
                WindowFocusedEvent e;
                data.eventfn(e);
            }
            else {
                WindowUnFocusedEvent e;
                data.eventfn(e);
            }
        });
    }

    // Key Events
    {
        glfwSetKeyCallback(m_handle, [](GLFWwindow* owner, int key, int scancode, int action, int mods) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            switch( action ) {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e(key, 0);
                    data.eventfn(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e(key);
                    data.eventfn(e);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e(key, 1);
                    data.eventfn(e);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_handle, [](GLFWwindow* owner, unsigned int keycode) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            KeyTypedEvent e(keycode);
            data.eventfn(e);
        });
    }


    // Mouse Events
    {
        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* owner, int button, int action, int mods) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            switch( action ) {
            case GLFW_PRESS:
            {
                MousePressedEvent e(button, mods);
                data.eventfn(e);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseReleasedEvent e(button);
                data.eventfn(e);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_handle, [](GLFWwindow* owner, double xoffset, double yoffset) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            MouseScrolledEvent e(xoffset, yoffset);
            data.eventfn(e);
        });

        glfwSetCursorPosCallback(m_handle, [](GLFWwindow* owner, double xpos, double ypos) {
            Properties data = *(Properties*)glfwGetWindowUserPointer(owner);

            MouseMovedEvent e(xpos, ypos);
            data.eventfn(e);
        });
    }
}
