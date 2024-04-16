#include "Chunk.h"

Chunk::Chunk(Scene* scene, std::string name, glm::vec3 position, uint16_t resolution, float unitSize) :
    SceneObject(scene),
    m_name(name),
    m_resolution(resolution),
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

    // temp
    float halfRes = get_resolution() / 2.f;

    Mesh& mesh = blueprint->mesh();
    mesh.set_vertices(std::vector<Vertex>(m_points.size()), 0);

    for( int x = 0; x < get_resolution(); x++ )
    {
        for( int y = 0; y < get_resolution(); y++ )
        {
            for( int z = 0; z < get_resolution(); z++ )
            {
                size_t index = coords_to_index(x, y, z);

                glm::vec3 position{ -halfRes + x, -halfRes + y, -halfRes + z };
                position *= unitSize;
                glm::vec3 colour{ rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX) };
                mesh.set_vertex(index, { position, colour }, 0);
            }
        }
    }

    int randMax{ 100 };
    int triangleCount = static_cast<int>((rand() / static_cast<float>(RAND_MAX)) * randMax) + 1;

    std::vector<uint16_t> triangles;
    for( int i = 0; i < triangleCount; i++ )
    {
        triangles.push_back(static_cast<uint16_t>((rand() / static_cast<float>(RAND_MAX)) * (m_points.size()-1)));
        triangles.push_back(static_cast<uint16_t>((rand() / static_cast<float>(RAND_MAX)) * (m_points.size()-1)));
        triangles.push_back(static_cast<uint16_t>((rand() / static_cast<float>(RAND_MAX)) * (m_points.size()-1)));
    }
    mesh.set_indices(triangles);
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

}

size_t Chunk::coords_to_index(int x, int y, int z) const
{
    size_t retval = 0;
    retval += z * get_resolution() * get_resolution();
    retval += y * get_resolution();
    retval += x;
    return retval;
}