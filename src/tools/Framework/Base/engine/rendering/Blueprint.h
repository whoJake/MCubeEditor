#pragma once

#include "common/stdincludes.h"
#include "common/spatial.h"
#include "Mesh.h"
#include "MeshRenderer.h"

class Blueprint
{
public:
    Blueprint(const std::string_view& name);
    ~Blueprint() = default;
    Blueprint(Blueprint&&) = default;
    Blueprint(const Blueprint&) = default;

    Blueprint& operator=(Blueprint&&) = delete;
    Blueprint& operator=(const Blueprint&&) = delete;

    const std::string_view& get_name() const;

    AABoundingBox<> get_bounds() const;

    Mesh& mesh();

    MeshRenderer& mesh_renderer();

    inline bool operator==(const Blueprint& other)
    {
        return other.m_nameHash == m_nameHash;
    }
private:
    std::string_view m_name;
    size_t m_nameHash{ 0 };
    AABoundingBox<> m_boundingBox{ };
    Mesh m_mesh{ };
    MeshRenderer m_meshRenderer;
};