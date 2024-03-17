#include "EntityProxy.h"

EntityProxy::EntityProxy(Entity* entity) :
    m_entity(entity),
    m_transform(entity->transform()),
    m_blueprintProxy(entity->get_blueprint_id())
{ }

void EntityProxy::sync()
{
    m_transform = m_entity->transform();
    m_blueprintProxy = m_entity->get_blueprint_id();
}