#include "Scene.h"

Scene::Scene(const std::string_view& name) :
    m_name(name)
{ }

const std::string_view& Scene::get_name() const
{
    return m_name;
}

const std::unordered_map<entid_t, Entity>& Scene::get_scene_entities()
{
    return m_entities;
}

void Scene::request_create_entity(Entity&& entity)
{
    std::lock_guard<std::mutex> lock(m_entityCreationMutex);
    m_entityCreationQueue.push(std::move(entity));
}

void Scene::request_destroy_entity(entid_t entity)
{
    std::lock_guard<std::mutex> lock(m_entityDestructionMutex);
    m_entityDestructionQueue.push(entity);
}

void Scene::request_create_blueprint(Blueprint&& blueprint)
{
    std::lock_guard<std::mutex> lock(m_blueprintCreationMutex);
    m_blueprintCreationQueue.push(std::move(blueprint));
}

Blueprint* Scene::get_blueprint(bpid_t id)
{
    auto it = m_blueprints.find(id);
    if( it != m_blueprints.end() )
    {
        return &(it->second).blueprint;
    }

    return nullptr;
}

Entity* Scene::get_entity(entid_t id)
{
    auto it = m_entities.find(id);
    if( it != m_entities.end() )
    {
        return &(it->second);
    }

    return nullptr;
}

void Scene::update_physics()
{ }

void Scene::update_gamestate()
{ }

void Scene::sync_proxies()
{ 
    // Maybe setup as task?

    for( auto& entity : m_entities )
    {
        auto entit = m_entityProxies.find(entity.second.get_id());
        auto bpit = m_blueprintProxies.find(entity.second.get_blueprint_id());

        if( entit != m_entityProxies.end() )
        {
            entit->second.sync();
        }

        if( bpit != m_blueprintProxies.end() )
        {
            auto it = m_blueprints.find(entity.second.get_blueprint_id());

            if( it != m_blueprints.end() )
            {
                bpit->second.sync();
            }
        }
    }
}

void Scene::resolve_creation_queue()
{
    // Create blueprints first
    {
        std::lock_guard<std::mutex> lock(m_blueprintCreationMutex);
        for( ; !m_blueprintCreationQueue.empty(); m_blueprintCreationQueue.pop() )
        {
            bpid_t insertId = m_blueprintCreationQueue.front().get_id();
            if( !m_blueprints.contains(insertId) )
            {
                m_blueprints.emplace(std::piecewise_construct,
                                     std::tuple(insertId),
                                     std::tuple(std::move(m_blueprintCreationQueue.front()), 0));
                // m_blueprints[insertId] = { std::move(m_blueprintCreationQueue.front()), 0 };
            }

            if( !m_blueprintProxies.contains(insertId) )
            {
                m_blueprintProxies.emplace(std::piecewise_construct,
                                           std::tuple(insertId),
                                           std::tuple(&(m_blueprints[insertId].blueprint)));
            }
        }
    }

    // Create entities
    {
        std::lock_guard<std::mutex> lock(m_entityCreationMutex);
        for( ; !m_entityCreationQueue.empty(); m_entityCreationQueue.pop() )
        {
            entid_t insertId = m_entityCreationQueue.front().get_id();
            Entity& insertedEntity = m_entities.emplace(std::piecewise_construct,
                                                        std::tuple(insertId),
                                                        std::tuple(std::move(m_entityCreationQueue.front()))).first->second;

            m_entityProxies.emplace(std::piecewise_construct,
                                    std::tuple(insertId),
                                    std::tuple(&insertedEntity));

            auto it = m_blueprints.find(insertedEntity.get_blueprint_id());
            if( it != m_blueprints.end() )
            {
                it->second.references++;
            }
            else
            {
                // warn that entity has been created without blueprint
            }
        }
    }
}

void Scene::resolve_destruction_queue()
{
    std::lock_guard<std::mutex> lock(m_entityDestructionMutex);
    for( ; !m_entityDestructionQueue.empty(); m_entityDestructionQueue.pop() )
    {
        auto it = m_entities.find(m_entityDestructionQueue.front());
        if( it != m_entities.end() )
        {
            // Get its bpid and erase entity
            bpid_t blueprint = it->second.get_blueprint_id();
            m_entities.erase(it);

            // Remove a reference from blueprint
            auto bpit = m_blueprints.find(blueprint);
            if( bpit != m_blueprints.end() )
            {
                // If new reference count is <= 0, destroy that blueprint
                if( --bpit->second.references <= 0 )
                {
                    m_blueprints.erase(bpit);
                    auto bpit2 = m_blueprintProxies.find(blueprint);
                    if( bpit2 != m_blueprintProxies.end() )
                    {
                        m_blueprintProxies.erase(bpit2);
                    }
                }
            }
        }

        // Destroy associating proxy
        auto it2 = m_entityProxies.find(m_entityDestructionQueue.front());
        if( it2 != m_entityProxies.end() )
        {
            m_entityProxies.erase(it2);
        }
    }
}