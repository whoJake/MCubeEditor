#pragma once

#include "applications/WindowedApplication.h"
#include "scene/Scene.h"

class MCubeEditorApp : public WindowedApplication
{
public:
    MCubeEditorApp();
    ~MCubeEditorApp();

    void on_app_startup();

    void update(double deltaTime);
private:
    std::unique_ptr<Scene> m_scene;
};