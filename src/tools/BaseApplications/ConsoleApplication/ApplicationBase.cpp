#include "ApplicationBase.h"

ApplicationBase::ApplicationBase() :
    m_exitFlags(ExitFlagBits::Success),
    m_commandArgs()
{ }

ApplicationBase::~ApplicationBase()
{
    app_shutdown();
}

ExitFlags ApplicationBase::run(int argc, const char* argv[])
{
    std::vector<std::string> argList(argc);
    for( int i = 0; i < argc; i++ )
    {
        argList[i] = std::string(argv[i]);
    }

    m_commandArgs = CommandArgs(argList);
    
    ExitFlags exitFlags;
    try
    {
        exitFlags = app_main();
    }
    catch( std::exception ex )
    {
        exitFlags |= ExitFlagBits::Failure;
    }

    return m_exitFlags;
}

void ApplicationBase::app_shutdown(ExitFlags exitFlags)
{

    m_exitFlags |= exitFlags;

    // Flush and destroy log

    on_app_shutdown();
}
