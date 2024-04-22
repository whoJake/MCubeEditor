#pragma once

#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Entity.h"
#include "SceneObject.h"

extern float g_chunkMarchingCubeThreshold;

class Chunk : public SceneObject
{
public:
    Chunk(Scene* scene, std::string name, glm::vec3 position, uint16_t resolution, float unitSize);
    Chunk(Chunk&&) = delete;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    ~Chunk();
    
    Mesh& mesh();

    Transform& transform();

    void set_point(int x, int y, int z, float value);
    void set_point(glm::uvec3 pos, float value);

    float get_point(int x, int y, int z) const;
    float get_point(glm::uvec3 pos) const;

    void recalculate_mesh();
private:
    size_t coords_to_index(int x,  int y, int z) const;
    size_t get_resolution() const;
private:
    std::string m_name;
    bpid_t m_blueprint{ 0 };
    entid_t m_entity{ 0 };

    std::vector<float> m_points;
    uint16_t m_resolution;
    float m_unitSize;

    glm::vec3 m_colour;
};