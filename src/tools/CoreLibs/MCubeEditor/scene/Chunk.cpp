#include "Chunk.h"

#include "mcube/LookupData.h"
#include "threading/JobDispatcher.h"

PARAM(marching_cube_threshold);
PARAM(disable_marching_cube_interpolation);
PARAM(marching_cube_resolution);

Chunk::Chunk(Scene* scene, std::string name, glm::vec3 origin, glm::vec3 size) :
    SceneObject(scene),
    m_name(name),
    m_size(size)
{
    Blueprint bp(m_name);
    m_blueprint = bp.get_id();

    Blueprint* blueprint = get_scene()->request_create_blueprint(std::move(bp));

    Entity ent(m_blueprint);
    m_entity = ent.get_id();
    Entity* entity = get_scene()->request_create_entity(std::move(ent));
    entity->transform().position() = origin;
    entity->transform().scale() = m_size;

    create_data_backed_volume();
    m_colour = { (rand() % 255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f };
    set_mesh_to_volume(blueprint);
}

Chunk::~Chunk()
{
    get_scene()->request_destroy_entity(m_entity);
    get_scene()->request_destroy_blueprint(m_blueprint);
}

Mesh* Chunk::mesh() const
{
    Blueprint* blueprint = get_scene()->get_blueprint(m_blueprint);
    if( blueprint )
    {
        return &blueprint->mesh();
    }
    return nullptr;
}

Transform* Chunk::transform() const
{
    Entity* entity = get_scene()->get_entity(m_entity);
    if( entity )
    {
        return &entity->transform();
    }
    return nullptr;
}

void Chunk::sphere_edit(glm::vec3 pos, float radius, float deltaTime, bool addition)
{
    if( !m_volume )
    {
        // No volume set to edit
        return;
    }

    if( !get_bounds().contains(pos, radius) )
    {
        // no influence
        return;
    }

    // transform sphere to local
    m_volume->add_local_sphere((pos - get_origin()) / m_size, radius / m_size.x, deltaTime * 10.f * (addition ? 1.f : -1.f), g_useMultithreading);

    set_mesh_to_volume();
}

AABoundingBox<float> Chunk::get_bounds() const
{
    glm::vec3 origin = get_origin();
    return AABoundingBox<float>{ origin, origin + m_size };
}

glm::vec3 Chunk::get_origin() const
{
    return transform()->position();
}

glm::vec3 Chunk::get_centre() const
{
    return get_origin() + (m_size / 2.f);
}

void Chunk::create_data_backed_volume(uint32_t resolution)
{
    glm::uvec3 dimensions{ resolution, resolution, resolution };
    if( m_volume && m_volume->get_dimensions() == dimensions )
    {
        // No need to recreate
        return;
    }

    m_volume = std::make_unique<mcube::Volume<float>>(dimensions, 0.f, 1.f, 0.5f);
    m_currentResolution = resolution;
}

void Chunk::set_mesh_to_volume(Blueprint* blueprint)
{
    if( !m_volume )
    {
        return;
    }

    Mesh* bpmesh = nullptr;
    if( blueprint )
    {
        bpmesh = &blueprint->mesh();
    }
    else
    {
        bpmesh = mesh();
    }

    if( !bpmesh )
    {
        return;
    }

    mcube::CalculationFlags flags = mcube::CalculationFlagBits::MESH;
    if( Param_disable_marching_cube_interpolation.get() )
    {
        flags |= mcube::CalculationFlagBits::NO_INTERPOLATION;
    }
    if( g_useMultithreading )
    {
        flags |= mcube::CalculationFlagBits::MULTITHREADED;
    }
    mcube::MeshData data = m_volume->calculate(flags);
    std::vector<Vertex> vertices;
    vertices.reserve(data.vertices.size());

    for( size_t i = 0; i < data.vertices.size(); i++ )
    {
        if( flags & mcube::CalculationFlagBits::NORMALS )
        {
            vertices.push_back({ data.vertices.at(i), data.normals.at(i), m_colour });
        }
        else
        {
            vertices.push_back({ data.vertices.at(i), { 0.f, 0.f, 0.f }, m_colour });
        }
    }

    bpmesh->set_vertices(vertices, 0);

    std::vector<uint16_t> indices;
    indices.reserve(data.indices.size());
    for( size_t i = 0; i < data.indices.size(); i++ )
    {
        indices.push_back(data.indices.at(i));
    }

    bpmesh->set_indices(indices);
    bpmesh->recalculate_normals();
}