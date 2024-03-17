#include "BlueprintProxy.h"

BlueprintProxy::BlueprintProxy(Blueprint* blueprint) :
    m_blueprint(blueprint),
    m_mesh(blueprint->mesh()),
    m_materialProxy(0)
{
    // Make sure dirty is true so that we don't forget to make a buffer on the first frame
    m_mesh.set_dirty(true);
}

void BlueprintProxy::sync()
{
    if( m_blueprint->mesh().get_dirty() )
    {
        // Copy mesh over if its dirty
        m_mesh.set_vertices(m_blueprint->mesh().get_vertices());
        m_mesh.set_indices(m_blueprint->mesh().get_indices());
        m_blueprint->mesh().set_dirty(false);
    }
}