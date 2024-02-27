#include "MCubeEditorApp.h"

#include "common/stdincludes.h"

#define OPTION_OPENFILE "file"

MCubeEditorApp::MCubeEditorApp() :
    WindowedApplication()
{
    Option openFile
    {
        OPTION_OPENFILE,
        1,
        OptionFlagBits::ARGUMENT_OPTIONAL | OptionFlagBits::OPTIONS_REQUIRED
    };

    get_config().get_arg_provider().register_option(openFile);
}

MCubeEditorApp::~MCubeEditorApp()
{

}

void MCubeEditorApp::on_app_startup()
{
    // Do initial startup

    if( get_config().get_arg_provider().has_argument(OPTION_OPENFILE) )
    {
        // Open scene from file
    }
    else
    {
        // Open empty scene
    }
}

void MCubeEditorApp::update(double deltaTime)
{

}

// ### Entry Point ###
int main(int argc, const char* argv[])
{
    MCubeEditorApp app;
    int exitCode = app.run(argc, argv);
    return exitCode;
}