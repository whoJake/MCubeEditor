#include "ApplicationBase.h"

ApplicationBase::ApplicationBase() :
    m_exitFlags(ExitFlagBits::Success)
{ }

ApplicationBase::~ApplicationBase()
{
    app_shutdown();
}

int ApplicationBase::run(int argc, const char* argv[])
{
    if( init_global_log() )
    {
        return ExitFlagBits::InitFailure;
    }

    Param::set_executable(argv[0]);
    Param::add_params(argc-1, &argv[1]);

    std::vector<std::string> argList(argc);
    for( int i = 0; i < argc; i++ )
    {
        argList[i] = std::string(argv[i]);
    }

    ExitFlags exitFlags;
    exitFlags = app_main();
    
    // Properly log exit flags.
    return m_exitFlags;
}

void ApplicationBase::app_shutdown(ExitFlags exitFlags)
{

    m_exitFlags |= exitFlags;

    // Flush and destroy log

    on_app_shutdown();
}

