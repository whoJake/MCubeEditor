#pragma once

#include "common/stdincludes.h"
#include "Entity.h"

class Scene
{
public:
    Scene();
    ~Scene() = default;

    Scene(Scene&&) = default;
    Scene(const Scene&) = default;

    Scene& operator=(Scene&&) = delete;
    Scene& operator=(const Scene&) = delete;

    std::vector<Entity*> get_scene_entities();

    void insert_entity(const Entity& entity);
private:
    std::vector<Entity> m_entities;
};