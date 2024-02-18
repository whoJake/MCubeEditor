#pragma once

#include "ApplicationBase.h"
#include "platform/events/Event.h"
#include "vulkan/rendering/RenderContext.h"

class WindowedApplication: public ApplicationBase
{
public:
    WindowedApplication();
    WindowedApplication(const Window::Properties& properties);
    ~WindowedApplication();

    ExitFlags app_main();

    void on_event(Event& e);

    vk::RenderContext& get_render_context();

    Window& get_window();
private:
    bool create_window();
    bool create_window(Window::Properties& properties);

    bool create_render_context();
private:
    std::unique_ptr<Window> m_window;
    Window::Properties m_windowProperties;

    std::unique_ptr<vk::RenderContext> m_renderContext;
};