#include "Scene.h"

Scene::Scene() :
    m_entities()
{ }

std::vector<Entity*> Scene::get_scene_entities()
{
    std::vector<Entity*> result;
    result.reserve(m_entities.size());
    for( Entity& entity : m_entities )
    {
        result.push_back(&entity);
    }
    return result;
}

void Scene::insert_entity(const Entity& entity)
{
    m_entities.push_back(entity);
}