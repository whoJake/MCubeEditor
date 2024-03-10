#pragma once

#include "common/stdincludes.h"
#include "common/spatial.h"

class Blueprint;

class Entity
{
public:
    Entity() = delete;
    Entity(std::shared_ptr<Blueprint> blueprint,
           const Transform& transform);
    Entity(std::shared_ptr<Blueprint> blueprint,
        const glm::vec3& position = { 0.f, 0.f, 0.f },
        const glm::vec3& scale    = { 1.f, 1.f, 1.f },
        const glm::quat& rotation = glm::quat({ 0.f, 0.f, 0.f }));
    Entity(std::shared_ptr<Blueprint> blueprint,
           const glm::vec3& position = { 0.f, 0.f, 0.f },
           const glm::vec3& scale    = { 1.f, 1.f, 1.f },
           const glm::vec3& rotation = { 0.f, 0.f, 0.f });
    ~Entity() = default;

    Entity(Entity&&) = default;
    Entity(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
    Entity& operator=(const Entity&) = default;

    Blueprint& blueprint();

    Transform& transform();

    AABoundingBox<> get_bounds() const;
private:
    std::shared_ptr<Blueprint> m_blueprint;
    Transform m_transform;
};