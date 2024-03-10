#include "Entity.h"
#include "engine/rendering/Blueprint.h"

Entity::Entity(std::shared_ptr<Blueprint> blueprint,
               const glm::vec3& position,
               const glm::vec3& scale,
               const glm::vec3& rotation) :
    m_blueprint(blueprint),
    m_transform(position, scale, glm::quat(rotation))
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