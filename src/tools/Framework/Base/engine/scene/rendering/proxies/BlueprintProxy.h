#pragma once

#include "common/mathplus.h"
#include "engine/scene/gameplay/Blueprint.h"
#include "engine/scene/gameplay/Mesh.h"

class BlueprintProxy
{
public:
    BlueprintProxy(Blueprint* blueprint);
    ~BlueprintProxy() = default;

    void sync();
private:
    Blueprint* m_blueprint;
    Mesh m_mesh;
    uint32_t m_materialProxy;
};