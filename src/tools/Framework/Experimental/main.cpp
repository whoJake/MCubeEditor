#include "applications/WindowedApplication.h"
#include "platform/Window.h"
#include <iostream>

int main(int argc, const char* argv[])
{
    Window::Properties winProperties{ };

    WindowedApplication app(winProperties);
    int exitcode = app.run(argc, argv);

    return exitcode;
}