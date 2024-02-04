#pragma once

#include "ApplicationBase.h"
#include "platform/events/Event.h"

class WindowedApplication: public ApplicationBase
{
public:
    WindowedApplication();
    ~WindowedApplication();

    ExitFlags app_main();

    void on_event(Event& e);
};