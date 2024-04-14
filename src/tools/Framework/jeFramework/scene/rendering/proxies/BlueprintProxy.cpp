#include "BlueprintProxy.h"

BlueprintProxy::BlueprintProxy(Blueprint* blueprint) :
    m_blueprint(blueprint),
    m_mesh(blueprint->mesh()),
    m_id(blueprint->get_id()),
    m_materialProxy(0)
{
    // Make sure dirty is true so that we don't forget to make a buffer on the first frame
    m_mesh.set_index_dirty(true);
    for( uint32_t i = 0; i < m_mesh.get_vertex_buffer_count(); i++ )
    {
        m_mesh.set_vertex_dirty(i, true);
    }
}

void BlueprintProxy::sync()
{
    m_mesh.set_index_dirty(m_blueprint->mesh().get_index_dirty());
    if( m_blueprint->mesh().get_index_dirty() )
    {
        m_mesh.set_indices(m_blueprint->mesh().get_indices());
        m_blueprint->mesh().set_index_dirty(false);
    }

    for( uint32_t i = 0; i < m_mesh.get_vertex_buffer_count(); i++ )
    {
        m_mesh.set_vertex_dirty(i, m_blueprint->mesh().get_vertex_dirty(i));
        if( m_blueprint->mesh().get_vertex_dirty(i) )
        {
            m_mesh.set_vertices(m_blueprint->mesh().get_vertices(i), i);
            m_blueprint->mesh().set_vertex_dirty(i, false);
        }
    }
}

bpid_t BlueprintProxy::get_id() const
{
    return m_id;
}

const Mesh& BlueprintProxy::get_mesh() const
{
    return m_mesh;
}