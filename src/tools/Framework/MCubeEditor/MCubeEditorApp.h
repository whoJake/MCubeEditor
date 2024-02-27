#pragma once

#include "applications/WindowedApplication.h";

class MCubeEditorApp : public WindowedApplication
{
public:
    MCubeEditorApp();
    ~MCubeEditorApp();

    void on_app_startup();

    void update(double deltaTime);
private:

};