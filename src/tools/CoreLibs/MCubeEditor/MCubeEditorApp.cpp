#include "MCubeEditorApp.h"

#include "data/spatial.h"
#include "input/Input.h"
#include "platform/events/Event.h"

#include "rendering/RenderContext.h"
#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Primitives.h"
#include "scene/gameplay/Entity.h"

#include "mcube/Volume.h"

PARAM(open_scene);

#define DEFAULT_START_CHUNKS_PER_AXIS 3
PARAM(start_chunks_per_axis);

#define DEFAULT_USE_MULTITHREADING false
bool g_useMultithreading{ DEFAULT_USE_MULTITHREADING };
PARAM(use_multithreading);

MCubeEditorApp::MCubeEditorApp() :
    WindowedApplication()
{ }

MCubeEditorApp::~MCubeEditorApp()
{

}

void MCubeEditorApp::on_app_startup()
{
    if( Param_use_multithreading.get() )
    {
        g_useMultithreading = true;
        int temp;
        if( Param_use_multithreading.get_int(&temp) )
        {
            g_useMultithreading = static_cast<bool>(temp);
        }
    }

    initialize_scene();
    JobDispatch::initialize();

    m_renderer = std::make_unique<Renderer>(get_render_context());

    if( Param_open_scene.get() )
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
    static double dt = deltaTime;
    dt = dt * 0.95 + deltaTime * 0.05;
    get_window().set_title(std::format("fps: {}",  static_cast<uint32_t>(1.0 / dt)));

    parse_input(deltaTime);

    if( g_useMultithreading )
    {
        std::function<void()> updateFunc = std::bind(&MCubeEditorApp::update_scene, this, deltaTime);
        std::function<void()> renderFunc = std::bind(&MCubeEditorApp::render_scene, this);

        std::atomic<uint32_t>* updateJob = JobDispatch::execute(updateFunc);
        std::atomic<uint32_t>* renderJob = JobDispatch::execute(renderFunc);

        while( (*updateJob).load() != 0 || (*renderJob).load() != 0 )
        { }
        JobDispatch::reset_counters();
    }
    else
    {
        update_scene(deltaTime);
        render_scene();
    }

    m_scene->resolve_creation_queue();
    m_scene->resolve_destruction_queue();

    m_scene->sync_proxies();
    Input::tick();
}

bool MCubeEditorApp::on_window_resize(WindowResizeEvent& e)
{
    if( m_camera )
    {
        if( e.get_width() <= 0.f
         || e.get_height() <= 0.f )
        {
            return false;
        }

        m_camera->set_aspect(static_cast<float>(e.get_width()) / static_cast<float>(e.get_height()));
    }

    return false;
}

void MCubeEditorApp::initialize_scene()
{
    m_camera = std::make_unique<PerspectiveCamera>(90.f, 4.f / 3.f, 0.02f, 2000.f);
    m_camera->translate({ 0.f, 5.f, 30.f });

    m_scene = std::make_unique<Scene>(&get_render_context(), "TEST_SCENE");

    // Make cursor object
    Blueprint cursor("Cursor");
    cursor.mesh().set_vertices(s_verticesUnitCube, 0);
    cursor.mesh().set_indices(s_indicesUnitCube);

    Entity cursorEnt(cursor.get_id(), get_cursor_position());
    m_cursor = cursorEnt.get_id();
    
    m_scene->request_create_blueprint(std::move(cursor));
    m_scene->request_create_entity(std::move(cursorEnt));

    int beginChunksPerAxis{ 0 };
    if( !Param_start_chunks_per_axis.get_int(&beginChunksPerAxis) )
    {
        beginChunksPerAxis = DEFAULT_START_CHUNKS_PER_AXIS;
    }
    beginChunksPerAxis = std::max(beginChunksPerAxis, 1);
    m_chunksPerAxis = beginChunksPerAxis;

    glm::ivec3 offset{ m_chunksPerAxis / 2, m_chunksPerAxis / 2, m_chunksPerAxis / 2 };

    for( int x = 0; x < beginChunksPerAxis; x++ )
    {
        for( int y = 0; y < beginChunksPerAxis; y++ )
        {
            for( int z = 0; z < beginChunksPerAxis; z++ )
            {
                // idc about trying to centre around 0,0,0 lol
                glm::ivec3 chunkIndex{ x, y, z };
                create_chunk(chunkIndex - offset);
            }
        }
    }
}

void MCubeEditorApp::parse_input(double deltaTime)
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

    float speed = 10.f;
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
        float sensitivity = .2f;

        float mouseX = static_cast<float>(sensitivity * Input::get_mouse_move_horizontal());
        float mouseY = static_cast<float>(sensitivity * Input::get_mouse_move_vertical());

        m_camera->rotate(glm::angleAxis(glm::radians(mouseY), cameraRight));
        m_camera->rotate(glm::angleAxis(glm::radians(mouseX), glm::vec3(0.f, 1.f, 0.f)));
    }

    glm::vec3 translation = cameraRight * movement.x + cameraForward * movement.z + glm::vec3(0.f, 1.f, 0.f) * movement.y;

    m_camera->translate(translation);

    // Move cursor object
    float cursorDistMoveSpeed = 5.f;
    if( Input::get_key_down(KeyCode::Q) )
    {
        m_cursorDistanceFromCamera += cursorDistMoveSpeed * static_cast<float>(deltaTime);
    }
    if( Input::get_key_down(KeyCode::E) )
    {
        m_cursorDistanceFromCamera += cursorDistMoveSpeed * -static_cast<float>(deltaTime);
    }
    m_cursorDistanceFromCamera = std::clamp(m_cursorDistanceFromCamera, 2.f, 100.f);

    m_cursorScale += 0.1f * static_cast<float>(Input::get_mouse_scroll_vertical());
    m_cursorScale = std::clamp(m_cursorScale, 0.1f, 50.f);
}

void MCubeEditorApp::update_scene(double deltaTime)
{
    // Update cursor
    Entity* cursorEntity = m_scene->get_entity(m_cursor);
    if( cursorEntity )
    {
        cursorEntity->transform().position() = get_cursor_position();
        cursorEntity->transform().scale() = glm::vec3(m_cursorScale, m_cursorScale, m_cursorScale);
    }

    if( Input::get_mouse_button_down(0) || Input::get_mouse_button_down(2) )
    {
        for( auto& chunk : m_chunks )
        {
            chunk.second->sphere_edit(get_cursor_position(), m_cursorScale, static_cast<float>(deltaTime), !Input::get_mouse_button_down(2));
        }
    }
}

void MCubeEditorApp::render_scene()
{
    uint32_t renderFinished;
    m_renderer->dispatch_render({ m_scene->get_blueprint_proxies(), m_scene->get_entity_proxies() }, { m_camera.get() }, &renderFinished);
}

void MCubeEditorApp::create_chunk(glm::ivec3 index)
{
    if( m_chunks.find(index) != m_chunks.end() )
    {
        return;
    }

    glm::vec3 size{ 5.f, 5.f, 5.f };
    glm::vec3 origin{ index.x * size.x, index.y * size.y, index.z * size.z };

    m_chunks.insert(std::pair(index,
        std::make_unique<Chunk>(m_scene.get(), std::format("chunk:{}-{}-{}", index.x, index.y, index.z), origin, size)));
}

glm::vec3 MCubeEditorApp::get_cursor_position() const
{
    glm::vec3 forwardVector = glm::vec3(0.f, 0.f, -1.f) * m_camera->get_rotation();
    return m_camera->get_position() + (forwardVector * m_cursorDistanceFromCamera);
}

std::vector<VkPresentModeKHR> MCubeEditorApp::request_swapchain_present_mode() const
{
    return { VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
}

VkPhysicalDeviceFeatures MCubeEditorApp::request_physical_device_feature_set() const
{
    VkPhysicalDeviceFeatures features{ };
    if( Param_wireframe.get() )
    {
        features.fillModeNonSolid = true;
    }
    return features;
}

// ### Entry Point ###
int main(int argc, const char* argv[])
{
    MCubeEditorApp app;
    int exitCode = app.run(argc, argv);
    return exitCode;
}