#pragma once

#include "applications/WindowedApplication.h"
#include "platform/events/WindowEvent.h"

#include "engine/rendering/Camera.h"
#include "engine/scene/Scene.h"
#include "engine/rendering/Renderer.h"
// #include "scene/Scene.h"

class MCubeEditorApp : public WindowedApplication
{
public:
    MCubeEditorApp();
    ~MCubeEditorApp();

    void on_app_startup();

    void on_event(Event& e) override;

    void update(double deltaTime);
private:
    bool on_window_resize(WindowResizeEvent& e);
    void initialize_scene();
private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<PerspectiveCamera> m_camera;
    std::unique_ptr<Scene> m_scene;

    std::shared_ptr<Blueprint> m_blueprint;
    float m_currentUpEntityScale = 1.f;
    float m_currentDownEntityScale = 1.f;
};