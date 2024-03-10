#include "Entity.h"
#include "engine/rendering/Blueprint.h"

Entity::Entity(std::shared_ptr<Blueprint> blueprint,
               const Transform& transform) :
    m_blueprint(std::move(blueprint)),
    m_transform(transform)
{ }

Entity::Entity(std::shared_ptr<Blueprint> blueprint,
               const glm::vec3& position,
               const glm::vec3& scale,
               const glm::quat& rotation) :
    Entity(blueprint, Transform{ position, scale, rotation })
{ }

Entity::Entity(std::shared_ptr<Blueprint> blueprint,
               const glm::vec3& position,
               const glm::vec3& scale,
               const glm::vec3& rotation) :
    Entity(blueprint, position, scale, glm::quat(rotation))
{ }

Blueprint& Entity::blueprint()
{
    return *m_blueprint;
}

Transform& Entity::transform()
{
    return m_transform;
}

AABoundingBox<> Entity::get_bounds() const
{
    AABoundingBox<> blueprintBounds = m_blueprint->get_bounds();
    glm::vec3 diag = (blueprintBounds.max - blueprintBounds.min) * m_transform.scale();

    return { blueprintBounds.centre() - diag / 2.f,
             blueprintBounds.centre() + diag / 2.f };
}