#pragma once

#include "common/stdincludes.h"
#include "gameplay/Entity.h"
#include "gameplay/Blueprint.h"
#include "rendering/proxies/EntityProxy.h"
#include "rendering/proxies/BlueprintProxy.h"

class Scene
{
public:
    Scene(const std::string_view& name);
    ~Scene() = default;

    Scene(Scene&&) = default;
    Scene(const Scene&) = default;

    Scene& operator=(Scene&&) = delete;
    Scene& operator=(const Scene&) = delete;

    const std::string_view& get_name() const;

    const std::unordered_map<entid_t, Entity>& get_scene_entities();

    void request_create_entity(Entity&& entity);
    void request_destroy_entity(entid_t entity);

    void request_create_blueprint(Blueprint&& blueprint);

    Blueprint* get_blueprint(bpid_t id);
    Entity* get_entity(entid_t id);

    void update_physics();

    void update_gamestate();

    void sync_proxies();

    void resolve_creation_queue();
    void resolve_destruction_queue();
private:
    std::string_view m_name;

    struct ReferenceCountedBlueprint
    {
        Blueprint blueprint;
        uint32_t references;
    };

    std::unordered_map<bpid_t, ReferenceCountedBlueprint> m_blueprints;
    std::unordered_map<entid_t, Entity> m_entities;

    std::unordered_map<bpid_t, BlueprintProxy> m_blueprintProxies;
    std::unordered_map<entid_t, EntityProxy> m_entityProxies;

    std::queue<Entity> m_entityCreationQueue;
    std::mutex m_entityCreationMutex;

    std::queue<entid_t> m_entityDestructionQueue;
    std::mutex m_entityDestructionMutex;

    std::queue<Blueprint> m_blueprintCreationQueue;
    std::mutex m_blueprintCreationMutex;
};