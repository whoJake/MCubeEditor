#include "applications/WindowedApplication.h"
#include <iostream>

int main(int argc, const char* argv[])
{
    WindowedApplication app;
    int exitcode = app.run(argc, argv);

    return exitcode;
}