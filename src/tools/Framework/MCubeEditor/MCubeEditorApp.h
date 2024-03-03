#pragma once

#include "applications/WindowedApplication.h"
#include "platform/events/WindowEvent.h"

#include "scene/Camera.h"
#include "scene/Scene.h"

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
private:
    std::unique_ptr<PerspectiveCamera> m_camera;
    std::unique_ptr<Scene> m_scene;
};