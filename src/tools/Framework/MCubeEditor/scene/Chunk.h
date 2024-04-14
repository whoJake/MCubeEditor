#pragma once

#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Entity.h"
#include "scene/Scene.h"

#define DEFAULT_CHUNK_RESOLUTION 16

class Chunk
{
public:
    Chunk(Blueprint* blueprint, Entity* entity);
    
    Mesh& mesh();

    Transform& transform();

    void set_point(int x, int y, int z, float value);
    void set_point(glm::uvec3 pos, float value);

    float get_point(int x, int y, int z) const;
    float get_point(glm::uvec3 pos) const;

    void recalculate_mesh();
private:
    size_t coords_to_index(int x,  int y, int z) const;
private:
    Blueprint* m_blueprint;
    Entity* m_entity;

    std::vector<float> m_points;

    size_t m_resolution{ 0 };
public:
    static Chunk create_chunk(Scene* scene, const char* name);
    static void destroy_chunk(Scene* scene, Chunk* chunk);
};