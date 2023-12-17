#include "WindowedApplication.h"
#include "jclog.h"

#include "Windows.h"
#include <cstdlib>

WindowedApplication::WindowedApplication()
{ }

WindowedApplication::~WindowedApplication()
{ }

ExitFlags WindowedApplication::app_main()
{
    // Create log
    jclog::Log log;
    log.register_target(new jclog::ConsoleTarget());
    log.register_target(new jclog::FileTarget("D:\\Projects\\MCubeEditor\\logs\\log.txt"));

    for( int i = 0; i < 10000; i++ )
    {
        float zeroOne = static_cast<float>(std::rand()) / RAND_MAX;
        jclog::Level lev = (jclog::Level)(8 * zeroOne);

        float rand2 = static_cast<float>(std::rand()) / RAND_MAX;
        std::string logstr = std::format("{}", rand2);
        log.log(lev, "app", logstr.c_str());
    }

    return ExitFlagBits::Success;
}

int main(int argc, const char* argv[])
{
    WindowedApplication app;
    return app.run(argc, argv);
}