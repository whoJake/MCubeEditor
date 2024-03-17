#pragma once

#include "common/mathplus.h"
#include "engine/scene/gameplay/Blueprint.h"


class BlueprintProxy
{
public:

    inline void sync(Blueprint& blueprint) { };
private:
    
    uint32_t m_materialProxy;
};