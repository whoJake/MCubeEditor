#pragma once

#include "application/WindowedApplication.h"
#include "platform/events/WindowEvent.h"

#include "scene/gameplay/Camera.h"
#include "scene/Scene.h"
#include "scene/rendering/Renderer.h"
#include "scene/Chunk.h"
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

    void create_chunk(glm::ivec3 index);
private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<PerspectiveCamera> m_camera;
    std::unique_ptr<Scene> m_scene;

    bpid_t m_blueprint{ 0 };
    float m_currentUpEntityScale = 1.f;
    float m_currentDownEntityScale = 1.f;

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>> m_chunks;
    glm::ivec3 m_currentChunkPos{ 0, 0, 0 };
};