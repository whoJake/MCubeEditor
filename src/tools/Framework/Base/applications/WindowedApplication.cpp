#include "WindowedApplication.h"

#include "Windows.h"
#include <cstdlib>

WindowedApplication::WindowedApplication()
{ }

WindowedApplication::~WindowedApplication()
{ }

ExitFlags WindowedApplication::app_main()
{
    // Create log
    for( int i = 0; i < 10000; i++ )
    {
        float zeroOne = static_cast<float>(std::rand()) / RAND_MAX;
        jclog::Level lev = (jclog::Level)(8 * zeroOne);

        float rand2 = static_cast<float>(std::rand()) / RAND_MAX;
        std::string logstr = std::format("{}", rand2);
        m_log.log(lev, "app", logstr.c_str());
    }

    return ExitFlagBits::Success;
}