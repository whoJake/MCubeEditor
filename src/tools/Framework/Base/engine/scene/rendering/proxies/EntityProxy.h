#pragma once

#include "common/mathplus.h"
#include "engine/scene/gameplay/Entity.h"

class EntityProxy
{
public:

    inline void sync(Entity& entity){ };
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    uint32_t m_blueprintProxy;
};