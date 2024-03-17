#pragma once

#include "common/stdincludes.h"
#include "common/spatial.h"
#include "Mesh.h"
#include "engine/scene/rendering/MeshRenderer.h"

typedef uint32_t bpid_t;

class Blueprint
{
public:
    Blueprint(const std::string_view& name = "");
    ~Blueprint();
    Blueprint(Blueprint&&);
    Blueprint(const Blueprint&) = default;

    Blueprint& operator=(Blueprint&&) = default;
    Blueprint& operator=(const Blueprint&) = default;

    const std::string_view& get_name() const;

    AABoundingBox<> get_bounds() const;

    Mesh& mesh();

    MeshRenderer& mesh_renderer();

    bpid_t get_id() const;
private:
    std::string_view m_name;
    AABoundingBox<> m_boundingBox{ };
    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<MeshRenderer> m_meshRenderer;
    bpid_t m_id;
};