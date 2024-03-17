#pragma once

#include "common/mathplus.h"
#include "engine/scene/gameplay/Entity.h"
#include "common/spatial.h"

class EntityProxy
{
public:
    EntityProxy(Entity* entity);
    ~EntityProxy() = default;

    void sync();
private:
    Entity* m_entity;
    Transform m_transform;
    uint32_t m_blueprintProxy;
};