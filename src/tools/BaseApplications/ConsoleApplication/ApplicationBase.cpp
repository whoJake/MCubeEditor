#include "ApplicationBase.h"

ApplicationBase::ApplicationBase(const std::vector<std::string> argumentList)
    : m_Running(false)
{
    
}

ApplicationBase::~ApplicationBase()
{
    app_shutdown();
}

ExitFlags ApplicationBase::app_main()
{
    m_Running = true;

    // Create log

    return on_app_main();
}

void ApplicationBase::app_shutdown(ExitFlags exitcode)
{
    m_Running = false;
    m_Exitflags = exitcode;

    m_Exitflags |= on_app_shutdown();

    // Flush and destroy log

}

#ifndef APP_NO_MAIN
int main(int argc, const char* argv[])
{
    std::vector<std::string> argList;
    for( int i = 0; i < argc; i++ )
    {
        argList.push_back(argv[i]);
    }

    ApplicationBase* app = create_application(argList);
    try
    {
        int32_t exitcode = app->app_main();
        delete app;
        return exitcode;
    }
    catch( std::exception ex )
    {
        delete app;
        return -1;
    }
}
#endif