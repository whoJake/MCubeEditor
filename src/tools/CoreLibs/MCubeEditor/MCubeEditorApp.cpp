#include "MCubeEditorApp.h"

#include "data/spatial.h"
#include "input/Input.h"
#include "platform/events/Event.h"

#include "rendering/RenderContext.h"
#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Primitives.h"
#include "scene/gameplay/Entity.h"

PARAM(open_scene);

#define DEFAULT_START_CHUNKS_PER_AXIS 3
PARAM(start_chunks_per_axis);

#define DEFAULT_CHUNK_RESOLUTION 16
PARAM(chunk_resolution);

#define DEFAULT_UNIT_SIZE 1
PARAM(chunk_unit_size);

#define DEFAULT_USE_MULTITHREADING false
bool g_useMultithreading{ DEFAULT_USE_MULTITHREADING };
PARAM(use_multithreading);

bool firstFrameHack = true;

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

    if( firstFrameHack )
    {
        firstFrameHack = false;
        for( auto& chunk : m_chunks )
        {
            chunk.second->recalculate_mesh();
        }
    }

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
    m_camera->translate({ 0.f, 2.f, 20.f });

    m_scene = std::make_unique<Scene>(&get_render_context(), "TEST_SCENE");

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

    if( Input::get_key_down(KeyCode::Down) )
    {
        g_chunkMarchingCubeThreshold -= 0.2f * static_cast<float>(deltaTime);
        g_chunkMarchingCubeThreshold = std::clamp(g_chunkMarchingCubeThreshold, 0.f, 1.f);
    }
    if( Input::get_key_down(KeyCode::Up) )
    {
        g_chunkMarchingCubeThreshold += 0.2f * static_cast<float>(deltaTime);
        g_chunkMarchingCubeThreshold = std::clamp(g_chunkMarchingCubeThreshold, 0.f, 1.f);
    }

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
}

void MCubeEditorApp::update_scene(double deltaTime)
{
    if( firstFrameHack )
        return;

    if( g_useMultithreading )
    {
        glm::ivec3 offset{ m_chunksPerAxis / 2, m_chunksPerAxis / 2, m_chunksPerAxis / 2 };
        std::function<void(DispatchState)> updateChunksFunc = [&, offset](DispatchState state){
            glm::ivec3 args{ (state.jobIndex % m_chunksPerAxis), (state.jobIndex / (m_chunksPerAxis)) % m_chunksPerAxis, state.jobIndex / (m_chunksPerAxis * m_chunksPerAxis) };

            if( args.x >= m_chunksPerAxis || args.y >= m_chunksPerAxis || args.z >= m_chunksPerAxis )
                return;

            m_chunks.at(args - offset)->recalculate_mesh();
            };

        JobDispatch::dispatch_and_wait(static_cast<uint32_t>(m_chunks.size()), 5, updateChunksFunc);
    }
    else
    {
        for( auto& chunk : m_chunks )
        {
            chunk.second->recalculate_mesh_mt();
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

    int chunkResolution{ 0 };
    if( !Param_chunk_resolution.get_int(&chunkResolution) )
    {
        chunkResolution = DEFAULT_CHUNK_RESOLUTION;
    }
    chunkResolution = std::max(chunkResolution, 2);

    float unitSize{ 0 };
    if( !Param_chunk_resolution.get_float(&unitSize) )
    {
        unitSize = DEFAULT_UNIT_SIZE;
    }
    unitSize = std::max(unitSize, 0.01f);

    float chunkSize = chunkResolution * unitSize;
    glm::vec3 centre{ index.x * (chunkSize - 1), index.y * (chunkSize-1), index.z * (chunkSize-1) };

    m_chunks.insert(std::pair(index,
        std::make_unique<Chunk>(m_scene.get(), std::format("chunk:{}-{}-{}", index.x, index.y, index.z), centre, chunkResolution, unitSize)));
}

std::vector<VkPresentModeKHR> MCubeEditorApp::request_swapchain_present_mode() const
{
    return { VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
}

VkPhysicalDeviceFeatures MCubeEditorApp::request_physical_device_feature_set() const
{
    VkPhysicalDeviceFeatures features{ };
    features.fillModeNonSolid = true;
    return features;
}

// ### Entry Point ###
int main(int argc, const char* argv[])
{
    MCubeEditorApp app;
    int exitCode = app.run(argc, argv);
    return exitCode;
}