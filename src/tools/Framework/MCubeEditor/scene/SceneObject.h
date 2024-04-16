#pragma once

#include "scene/Scene.h"

class SceneObject
{
public:
    SceneObject(Scene* scene) :
        m_scene(scene)
    { }

    Scene* get_scene() const
    {
        return m_scene;
    }
private:
    Scene* m_scene;
};