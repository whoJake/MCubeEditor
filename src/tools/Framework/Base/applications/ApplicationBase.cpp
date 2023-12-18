#include "ApplicationBase.h"

ApplicationBase::ApplicationBase() :
    m_exitFlags(ExitFlagBits::Success),
    m_config(),
    m_log()
{ 
    m_log.register_target(new jclog::ConsoleTarget());
}

ApplicationBase::~ApplicationBase()
{
    app_shutdown();
}

int ApplicationBase::run(int argc, const char* argv[])
{
    std::vector<std::string> argList(argc);
    for( int i = 0; i < argc; i++ )
    {
        argList[i] = std::string(argv[i]);
    }
    
    try
    {
        m_config.get_arg_provider().assign_arguments(argList);
    }
    catch( std::exception ex )
    {
        // Log exception?
        // Display all valid/registered command arguments
        m_exitFlags |= ExitFlagBits::Failure;
        return m_exitFlags;
    }

    ExitFlags exitFlags;
    try
    {
        exitFlags = app_main();
    }
    catch( std::exception ex )
    {
        exitFlags |= ExitFlagBits::Failure;
    }


    // Properly log exit flags.
    return m_exitFlags;
}

void ApplicationBase::app_shutdown(ExitFlags exitFlags)
{

    m_exitFlags |= exitFlags;

    // Flush and destroy log

    on_app_shutdown();
}

jclog::Log& ApplicationBase::get_log()
{
    return m_log;
}
