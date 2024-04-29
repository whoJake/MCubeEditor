#pragma once

#include "application/WindowedApplication.h"
#include "platform/events/WindowEvent.h"

#include "scene/gameplay/Camera.h"
#include "scene/Scene.h"
#include "scene/rendering/Renderer.h"
#include "scene/Chunk.h"
#include "threading/JobDispatcher.h"
// #include "scene/Scene.h"

extern bool g_useMultithreading;

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

    void parse_input(double deltaTime);

    void update_scene(double deltaTime);
    void render_scene();

    void create_chunk(glm::ivec3 index);

    glm::vec3 get_cursor_position() const;
private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<PerspectiveCamera> m_camera;
    std::unique_ptr<Scene> m_scene;

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>> m_chunks;
    int m_chunksPerAxis{ 0 };
    float m_cursorDistanceFromCamera{ 5.f };
    float m_cursorScale{ 1.f };
    entid_t m_cursor{ 0 };
};