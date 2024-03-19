#pragma once

#include "applications/WindowedApplication.h"
#include "platform/events/WindowEvent.h"

#include "engine/scene/gameplay/Camera.h"
#include "engine/scene/Scene.h"
#include "engine/scene/rendering/Renderer.h"
// #include "scene/Scene.h"

class MCubeEditorApp : public WindowedApplication
{
public:
    MCubeEditorApp();
    ~MCubeEditorApp();

    void on_app_startup();

    void on_event(Event& e) override;

    void update(double deltaTime);
protected:
    std::vector<VkPresentModeKHR> request_swapchain_present_mode() const override;
    VkPhysicalDeviceFeatures request_physical_device_feature_set() const override;
private:
    bool on_window_resize(WindowResizeEvent& e);
    void initialize_scene();
private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<PerspectiveCamera> m_camera;
    std::unique_ptr<Scene> m_scene;

    bpid_t m_blueprint{ 0 };
    float m_currentUpEntityScale = 1.f;
    float m_currentDownEntityScale = 1.f;
};