#pragma once

#include "platform/Window.h"

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#include "glfw3.h"

class WindowGlfw : public Window {
public:
    WindowGlfw(jclog::Log& log, const Properties& properties);
    virtual ~WindowGlfw();

    void* get_native_handle() const;

    // VkSurfaceKHR create_surface(vulkan::Instance& instance) override;

    // std::vector<const char*> get_required_surface_extensions() const override;

    bool get_should_close() const;

    void close();

    void process_events();

    void set_title(const std::string& title);

    Position set_position(const Position& position);

    Extent set_size(const Extent& extent);

    void set_mode(const Mode& mode);
private:
    void setup_events() const;
private:
    GLFWwindow* m_handle;
};
