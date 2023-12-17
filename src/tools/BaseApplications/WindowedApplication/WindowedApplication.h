#pragma once

#include "ApplicationBase.h"
#include "glfw/glfw3.h"

class WindowedApplication: public ApplicationBase
{
public:
    WindowedApplication();
    ~WindowedApplication();

    ExitFlags app_main();
private:
};