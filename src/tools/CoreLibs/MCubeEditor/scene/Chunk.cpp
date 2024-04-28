#include "Chunk.h"

#include "MarchingCubeLookup.h"
#include "threading/JobDispatcher.h"

float g_chunkMarchingCubeThreshold = -1;

#define DEFAULT_MARCHING_CUBE_THRESHOLD 0.5
PARAM(marching_cube_threshold);

Chunk::Chunk(Scene* scene, std::string name, glm::vec3 position, uint16_t resolution, float unitSize) :
    SceneObject(scene),
    m_name(name),
    m_resolution(resolution),
    m_threshold(g_chunkMarchingCubeThreshold),
    m_unitSize(unitSize)
{
    m_points.resize(get_resolution() * get_resolution() * get_resolution());

    Blueprint bp(name);
    m_blueprint = bp.get_id();

    Blueprint* blueprint = get_scene()->request_create_blueprint(std::move(bp));

    Entity ent(m_blueprint);
    m_entity = ent.get_id();
    Entity* entity = get_scene()->request_create_entity(std::move(ent));
    entity->transform().position() = position;

    glm::vec3 centre{ get_resolution() / 2.f, get_resolution() / 2.f, get_resolution() / 2.f };
    for( int x = 0; x < get_resolution(); x++ )
    {
        for( int y = 0; y < get_resolution(); y++ )
        {
            for( int z = 0; z < get_resolution(); z++ )
            {
                glm::vec3 point{ x, y, z };
                float value = glm::distance(centre, point) / (get_resolution() / 1.5f);
                set_point(point, value);
            }
        }
    }

    m_colour = { (rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f };
}

Chunk::~Chunk()
{
    m_points.clear();
    get_scene()->request_destroy_entity(m_entity);
    get_scene()->request_destroy_blueprint(m_blueprint);
}

Mesh& Chunk::mesh()
{
    // cache this?
    return get_scene()->get_blueprint(m_blueprint)->mesh();
}

Transform& Chunk::transform()
{
    return get_scene()->get_entity(m_entity)->transform();
}

void Chunk::set_point(int x, int y, int z, float value)
{
    size_t index = coords_to_index(x, y, z);
    m_points.at(index) = value;
    if( m_points.at(index) != value )
    {
        m_points.at(index) = value;
        m_dirty = true;
    }
}

void Chunk::set_point(glm::uvec3 pos, float value)
{
    set_point(pos.x, pos.y, pos.z, value);
}

float Chunk::get_point(int x, int y, int z) const
{
    size_t index = coords_to_index(x, y, z);
    return m_points.at(index);
}

float Chunk::get_point(glm::uvec3 pos) const
{
    return get_point(pos.x, pos.y, pos.z);
}

size_t Chunk::get_resolution() const
{
    return static_cast<size_t>(m_resolution);
}

void Chunk::recalculate_mesh()
{
    float threshold = g_chunkMarchingCubeThreshold;
    if( threshold == -1 )
    {
        Param_marching_cube_threshold.get_float(&g_chunkMarchingCubeThreshold);
        threshold = g_chunkMarchingCubeThreshold;
    }

    if( !m_dirty && m_threshold == g_chunkMarchingCubeThreshold )
    {
        return;
    }

    m_threshold = threshold;

    MarchingCubeLookup* mcube = MarchingCubeLookup::instance();
    std::vector<Vertex> orderedVertexList;
    for( uint16_t x = 0; x < get_resolution() - 1; x++ )
    {
        for( uint16_t y = 0; y < get_resolution() - 1; y++ )
        {
            for( uint16_t z = 0; z < get_resolution() - 1; z++ )
            {
                recalculate_point_mesh({ x, y, z }, orderedVertexList);
            }
        }
    }

    mesh().set_vertices(orderedVertexList, 0);

    std::vector<uint16_t> indicies;
    indicies.resize(orderedVertexList.size());
    for( uint16_t i = 0; i < static_cast<uint16_t>(indicies.size()); i++ )
    {
        indicies.at(i) = i;
    }

    mesh().set_indices(indicies);
    m_dirty = false;
}

void Chunk::recalculate_mesh_mt()
{
    float threshold = g_chunkMarchingCubeThreshold;
    if( threshold == -1 )
    {
        Param_marching_cube_threshold.get_float(&g_chunkMarchingCubeThreshold);
        threshold = g_chunkMarchingCubeThreshold;
    }

    if( !m_dirty && m_threshold == g_chunkMarchingCubeThreshold )
    {
        return;
    }

    m_threshold = threshold;

    std::vector<Vertex> vertices;
    std::mutex lock;

    uint32_t maxCubes = m_resolution * m_resolution * m_resolution;
    uint32_t maxTris = maxCubes * 5;

    // Preallocate buffer so that we don't have to do big data moves
    vertices.reserve(maxTris);

    std::function<void(DispatchState)> job = [&](DispatchState state)
    {
            glm::uvec3 point{ (state.jobIndex % m_resolution), (state.jobIndex / (m_resolution)) % m_resolution, state.jobIndex / (m_resolution * m_resolution) };

            if( point.x >= m_resolution - 1u || point.y >= m_resolution - 1u || point.z >= m_resolution - 1u )
                return;

            recalculate_point_mesh(point, vertices, &lock);
    };

    JobDispatch::dispatch_and_wait(m_resolution * m_resolution * m_resolution, 25, job);

    vertices.shrink_to_fit();
    mesh().set_vertices(vertices, 0);

    std::vector<uint16_t> indicies;
    indicies.resize(vertices.size());
    for( uint16_t i = 0; i < static_cast<uint16_t>(indicies.size()); i++ )
    {
        indicies.at(i) = i;
    }

    mesh().set_indices(indicies);
    m_dirty = false;
}

void Chunk::recalculate_point_mesh(glm::uvec3 point, std::vector<Vertex>& vertexList, std::mutex* listMutex) const
{
    MarchingCubeLookup* mcube = MarchingCubeLookup::instance();

    // get corner state
    uint8_t cornerState{ 0 };
    for( uint32_t corner = 0; corner < 8; corner++ )
    {
        glm::ivec3 offset = mcube->get_corner_offset(corner);
        glm::uvec3 cornerPoint{ point.x + offset.x, point.y + offset.y, point.z + offset.z };

        if( get_point(cornerPoint) > m_threshold )
        {
            cornerState |= (1 << corner);
        }
    }

    // get active edges
    const std::array<int8_t, 16>& activeEdges = mcube->get_edges_for_state(cornerState);
    glm::vec3 chunkPhysicalSize{ (get_resolution() - 1) * m_unitSize, (get_resolution() - 1) * m_unitSize, (get_resolution() - 1) * m_unitSize };
    glm::vec3 chunkPhysicalOrigin = chunkPhysicalSize * -0.5f;

    glm::vec3 voxelPhysicalOrigin{ chunkPhysicalOrigin.x + (point.x * m_unitSize), chunkPhysicalOrigin.y + (point.y * m_unitSize), chunkPhysicalOrigin.z + (point.z * m_unitSize) };

    // iterate lookup table
    for( size_t edge = 0; edge < 16; edge += 3 )
    {
        if( activeEdges[edge] == -1 )
        {
            break;
        }

        std::pair<uint8_t, uint8_t> edgeA = mcube->get_corners_by_edge(activeEdges[edge]);
        std::pair<uint8_t, uint8_t> edgeB = mcube->get_corners_by_edge(activeEdges[edge + 1]);
        std::pair<uint8_t, uint8_t> edgeC = mcube->get_corners_by_edge(activeEdges[edge + 2]);

        // no interpolation
        glm::vec3 edgeAVertexA = mcube->get_corner_offset(edgeA.first);
        edgeAVertexA *= m_unitSize;
        edgeAVertexA += voxelPhysicalOrigin;

        glm::vec3 edgeAVertexB = mcube->get_corner_offset(edgeA.second);
        edgeAVertexB *= m_unitSize;
        edgeAVertexB += voxelPhysicalOrigin;

        Vertex edgeAVertex{ };
        edgeAVertex.position = (edgeAVertexA + edgeAVertexB) / 2.f;
        edgeAVertex.colour = m_colour;


        glm::vec3 edgeBVertexA = mcube->get_corner_offset(edgeB.first);
        edgeBVertexA *= m_unitSize;
        edgeBVertexA += voxelPhysicalOrigin;

        glm::vec3 edgeBVertexB = mcube->get_corner_offset(edgeB.second);
        edgeBVertexB *= m_unitSize;
        edgeBVertexB += voxelPhysicalOrigin;

        Vertex edgeBVertex{ };
        edgeBVertex.position = (edgeBVertexA + edgeBVertexB) / 2.f;
        edgeBVertex.colour = m_colour;


        glm::vec3 edgeCVertexA = mcube->get_corner_offset(edgeC.first);
        edgeCVertexA *= m_unitSize;
        edgeCVertexA += voxelPhysicalOrigin;

        glm::vec3 edgeCVertexB = mcube->get_corner_offset(edgeC.second);
        edgeCVertexB *= m_unitSize;
        edgeCVertexB += voxelPhysicalOrigin;

        Vertex edgeCVertex{ };
        edgeCVertex.position = (edgeCVertexA + edgeCVertexB) / 2.f;
        edgeCVertex.colour = m_colour;

        {
            size_t triangleBegin;
            if( listMutex )
            {
                std::unique_lock<std::mutex> lock(*listMutex);
                triangleBegin = vertexList.size();
                vertexList.resize(triangleBegin + 3);
            }
            else
            {
                triangleBegin = vertexList.size();
                vertexList.resize(triangleBegin + 3);
            }
            vertexList.at(triangleBegin) = edgeCVertex;
            vertexList.at(triangleBegin + 1) = edgeBVertex;
            vertexList.at(triangleBegin + 2) = edgeAVertex;
        }
    }
}

size_t Chunk::coords_to_index(int x, int y, int z) const
{
    size_t retval = 0;
    retval += z * get_resolution() * get_resolution();
    retval += y * get_resolution();
    retval += x;
    return retval;
}