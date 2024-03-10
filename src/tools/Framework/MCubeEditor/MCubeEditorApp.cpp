#include "MCubeEditorApp.h"

#include "common/stdincludes.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "engine/input/Input.h"
#include "platform/events/Event.h"

#include "vulkan/rendering/RenderContext.h"
#include "engine/rendering/Blueprint.h"
#include "engine/rendering/Primitives.h"
#include "engine/scene/Entity.h"

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
    /*
    m_scene = std::make_unique<Scene>();
    m_scene->create_material(get_render_context().get_device());
    */

    initialize_scene();
    m_renderer = std::make_unique<Renderer>(get_render_context());

    m_camera = std::make_unique<PerspectiveCamera>(90.f, 4.f / 3.f, 0.02f, 100.f);
    m_camera->translate({ 2.f, 2.f, 2.f });

    if( get_config().get_arg_provider().has_argument(OPTION_OPENFILE) )
    {
        // Open scene from file
    }
    else
    {
        // Open empty scene
    }
}

void MCubeEditorApp::on_event(Event& e)
{
    Input::register_event(e);

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(MCubeEditorApp::on_window_resize));
}

void MCubeEditorApp::update(double deltaTime)
{
    glm::vec3 movement{ };
    if( Input::get_key_down(KeyCode::A) )
        movement.x -= 1;
    if( Input::get_key_down(KeyCode::D) )
        movement.x += 1;
    if( Input::get_key_down(KeyCode::S) )
        movement.z += 1;
    if( Input::get_key_down(KeyCode::W) )
        movement.z -= 1;
    if( Input::get_key_down(KeyCode::Space) )
        movement.y += 1;
    if( Input::get_key_down(KeyCode::LeftShift) )
        movement.y -= 1;

    float speed = 5.f;
    movement *= speed * deltaTime;

    if( Input::get_mouse_button_pressed(1) )
    {
        Input::set_cursor_lock_state(get_window(), CursorLockState::LOCKED);
    }
    else if( Input::get_mouse_button_released(1) )
    {
        Input::set_cursor_lock_state(get_window(), CursorLockState::NONE);
    }

    glm::vec3 cameraForward = glm::vec3(0.f, 0.f, 1.f) * m_camera->get_rotation();
    glm::vec3 cameraRight = glm::vec3(1.f, 0.f, 0.f) * m_camera->get_rotation();
    
    if( Input::get_mouse_button_down(1) )
    {
        float sensitivity = 70.f;

        float mouseX = static_cast<float>(sensitivity * deltaTime * Input::get_mouse_move_horizontal());
        float mouseY = static_cast<float>(sensitivity * deltaTime * Input::get_mouse_move_vertical());

        m_camera->rotate(glm::angleAxis(glm::radians(mouseY), cameraRight));
        m_camera->rotate(glm::angleAxis(glm::radians(mouseX), glm::vec3(0.f, 1.f, 0.f)));
    }

    glm::vec3 translation = cameraRight * movement.x + cameraForward * movement.z + glm::vec3(0.f, 1.f, 0.f) * movement.y;

    m_camera->translate(translation);

    if( Input::get_key_pressed(KeyCode::Up) )
    {
        m_currentUpEntityScale++;
        Entity entity(m_blueprint);
        entity.transform().scale() *= glm::vec3(m_currentUpEntityScale, 1.f, m_currentUpEntityScale);
        entity.transform().translate({ 0.f, m_currentUpEntityScale * 1.2f - 1.f, 0.f });

        m_scene->insert_entity(entity);
    }

    if( Input::get_key_pressed(KeyCode::Down) )
    {
        m_currentDownEntityScale++;
        Entity entity(m_blueprint);
        entity.transform().scale() *= glm::vec3(1.f / m_currentDownEntityScale, 1.f, 1.f / m_currentDownEntityScale);
        entity.transform().translate({ 0.f, -m_currentDownEntityScale * 1.2f + 1.f, 0.f });

        m_scene->insert_entity(entity);
    }

    if( Input::get_key_pressed(KeyCode::Left) )
    {
        m_blueprint->mesh().set_vertices(s_verticesUnitPlane);
        m_blueprint->mesh().set_indices(s_indicesUnitPlane);
    }

    if( Input::get_key_pressed(KeyCode::Right) )
    {
        m_blueprint->mesh().set_vertices(s_verticesUnitCube);
        m_blueprint->mesh().set_indices(s_indicesUnitCube);
    }

    m_renderer->render_scene(*m_scene, *m_camera);

    /*
    m_scene->render(get_render_context(), *m_camera);
    */

    Input::tick();
}

bool MCubeEditorApp::on_window_resize(WindowResizeEvent& e)
{
    if( m_camera )
    {
        m_camera->set_aspect(static_cast<float>(e.get_width()) / static_cast<float>(e.get_height()));
    }

    return false;
}

void MCubeEditorApp::initialize_scene()
{
    m_scene = std::make_unique<Scene>();
    
    m_blueprint = std::make_shared<Blueprint>("TEST_CUBE");
    m_blueprint->mesh().set_vertices(s_verticesUnitCube);
    m_blueprint->mesh().set_indices(s_indicesUnitCube);

    Entity entity(m_blueprint);

    m_scene->insert_entity(entity);
}

// ### Entry Point ###
int main(int argc, const char* argv[])
{
    MCubeEditorApp app;
    int exitCode = app.run(argc, argv);
    return exitCode;
}