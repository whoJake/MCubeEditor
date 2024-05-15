#pragma once

#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Entity.h"
#include "SceneObject.h"

#include "mcube/Volume.h"

#define DEFAULT_MARCHING_CUBE_RESOLUTION 16
#define DEFAULT_MARCHING_CUBE_THRESHOLD 0.5

extern bool g_useMultithreading;

class Chunk : public SceneObject
{
public:
    Chunk(Scene* scene, std::string name, glm::vec3 origin, glm::vec3 size);
    Chunk(Chunk&&) = delete;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    ~Chunk();
    
    Mesh* mesh() const;
    Transform* transform() const;

    void sphere_edit(glm::vec3 pos, float radius, float deltaTime, bool addition);

    AABoundingBox<float> get_bounds() const;

    glm::vec3 get_origin() const;
    glm::vec3 get_centre() const;
private:
    void create_data_backed_volume(uint32_t resolution = DEFAULT_MARCHING_CUBE_RESOLUTION);

    void set_mesh_to_volume(Blueprint* blueprint = nullptr);
private:
    std::string m_name;
    bpid_t m_blueprint{ 0 };
    entid_t m_entity{ 0 };

    std::unique_ptr<mcube::Volume<float>> m_volume;
    glm::vec3 m_size;
    glm::vec3 m_colour;

    uint32_t m_currentResolution{ 0 };
};