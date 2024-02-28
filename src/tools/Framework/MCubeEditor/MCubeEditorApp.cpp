#include "MCubeEditorApp.h"

#include "common/stdincludes.h"
#include "glm.hpp"

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
    m_scene = std::make_unique<Scene>();
    m_scene->create_material(get_render_context().get_device());

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
    static glm::mat4 viewMatrix(0.f);

    m_scene->render(get_render_context(), viewMatrix);
}

// ### Entry Point ###
int main(int argc, const char* argv[])
{
    MCubeEditorApp app;
    int exitCode = app.run(argc, argv);
    return exitCode;
}