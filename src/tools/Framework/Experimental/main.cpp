#include "applications/WindowedApplication.h"

int main(int argc, const char* argv[])
{
    WindowedApplication app;
    int exitcode = app.run(argc, argv);

    return exitcode;
}