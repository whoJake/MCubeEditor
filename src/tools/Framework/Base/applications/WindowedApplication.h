#pragma once

#include "ApplicationBase.h"
#include "platform/events/Event.h"
#include "vulkan/rendering/RenderContext.h"
#include "vulkan/core/Pipeline.h"

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
protected:
    virtual std::vector<const char*> request_instance_extensions() const;

    virtual std::vector<const char*> request_validation_layers() const;

    virtual std::vector<const char*> request_device_extensions() const;

    virtual std::vector<VkPresentModeKHR> request_swapchain_present_mode() const;

    virtual std::vector<VkSurfaceFormatKHR> request_swapchain_format() const;
private:
    bool create_window();
    bool create_window(Window::Properties& properties);

    bool create_render_context();

    void create_vulkan_things();
private:
    std::unique_ptr<Window> m_window;
    Window::Properties m_windowProperties;

    std::unique_ptr<vk::Instance> m_vkInstance;
    std::unique_ptr<vk::Device> m_vkDevice;
    std::unique_ptr<vk::RenderContext> m_renderContext;

    struct
    {
        std::unique_ptr<vk::RenderPass> renderPass;
        std::unique_ptr<vk::PipelineLayout> pipelineLayout;
        std::unique_ptr<vk::Pipeline> pipeline;
        vk::PipelineState pipelineState;
    }m_vulkanThings;
};