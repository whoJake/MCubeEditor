#include "Chunk.h"

PARAM(chunk_resolution);

Chunk::Chunk(Blueprint* blueprint, Entity* entity) :
    m_blueprint(blueprint),
    m_entity(entity)
{
    if( !Param_chunk_resolution.get_int((int*)&m_resolution) )
    {
        m_resolution = DEFAULT_CHUNK_RESOLUTION;
    }

    m_points.resize(m_resolution * m_resolution * m_resolution);
}

Mesh& Chunk::mesh()
{
    return m_blueprint->mesh();
}

Transform& Chunk::transform()
{
    return m_entity->transform();
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

void Chunk::recalculate_mesh()
{

}

size_t Chunk::coords_to_index(int x, int y, int z) const
{
    size_t retval = 0;
    retval += z * m_resolution * m_resolution;
    retval += y * m_resolution;
    retval += x;
    return retval;
}

Chunk Chunk::create_chunk(Scene* scene, const char* name)
{
    Blueprint blueprint(name);
    bpid_t bpid = blueprint.get_id();

    Entity entity(bpid);
    entid_t entid = entity.get_id();

    scene->request_create_blueprint(std::move(blueprint));
    scene->request_create_entity(std::move(entity));
    scene->resolve_creation_queue();

    Chunk retval(scene->get_blueprint(bpid), scene->get_entity(entid));
    return retval;
}

void Chunk::destroy_chunk(Scene* scene, Chunk* chunk)
{
    scene->request_destroy_entity(chunk->m_entity->get_id());
    scene->request_destroy_blueprint(chunk->m_blueprint->get_id());

    chunk->m_entity = nullptr;
    chunk->m_blueprint = nullptr;
}