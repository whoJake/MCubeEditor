#pragma once

#include "scene/gameplay/Blueprint.h"
#include "scene/gameplay/Mesh.h"

class BlueprintProxy
{
public:
    BlueprintProxy(Blueprint* blueprint);
    ~BlueprintProxy() = default;

    void sync();

    bpid_t get_id() const;

    const Mesh& get_mesh() const;
private:
    Blueprint* m_blueprint;
    Mesh m_mesh;
    bpid_t m_id;
    uint32_t m_materialProxy;
};