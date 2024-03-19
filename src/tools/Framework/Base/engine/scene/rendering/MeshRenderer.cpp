#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh) :
    m_mesh(mesh)
{ }

MeshRenderer::~MeshRenderer()
{
    for( auto& buffer : m_vertexBuffers )
    {
        buffer.reset();
    }

    for( auto& buffer : m_indexBuffers )
    {
        buffer.reset();
    }
}

vk::Buffer& MeshRenderer::get_vertex_buffer(vk::RenderContext& context)
{
    m_vertexBuffers.resize(context.get_swapchain_properties().imageCount);
    m_staleVertexBuffers.resize(context.get_swapchain_properties().imageCount, true);

    uint32_t frameIndex = context.get_active_frame_index();
    uint32_t prevFrameIndex = frameIndex == 0
        ? context.get_swapchain_properties().imageCount - 1
        : frameIndex - 1;

    if( m_staleVertexBuffers[frameIndex] )
    {
        if( !m_staleVertexBuffers[prevFrameIndex]
          && m_vertexBuffers[prevFrameIndex] )
        {
            m_vertexBuffers[frameIndex] = m_vertexBuffers[prevFrameIndex];
            m_staleVertexBuffers[frameIndex] = false;
        }
        else
        {
            recreate_vertex_buffer(context.get_device(), m_vertexBuffers[frameIndex], m_mesh->get_vertices_size(0));
            for( int i = 0; i < m_staleVertexBuffers.size(); i++ )
            {
                m_staleVertexBuffers[i] = true;
            }
            m_staleVertexBuffers[frameIndex] = false;

            uint8_t* pData = m_vertexBuffers[frameIndex]->map();
            memcpy(pData, m_mesh->get_vertices(0).data(), m_mesh->get_vertices_size(0));
            m_vertexBuffers[frameIndex]->unmap();
        }
    }
    else
    {
        if( m_mesh->get_vertex_dirty(0) )
        {
            recreate_vertex_buffer(context.get_device(), m_vertexBuffers[frameIndex], m_mesh->get_vertices_size(0));
            for( int i = 0; i < m_staleVertexBuffers.size(); i++ )
            {
                m_staleVertexBuffers[i] = true;
            }
            m_staleVertexBuffers[frameIndex] = false;

            uint8_t* pData = m_vertexBuffers[frameIndex]->map();
            memcpy(pData, m_mesh->get_vertices(0).data(), m_mesh->get_vertices_size(0));
            m_vertexBuffers[frameIndex]->unmap();
        }

    }

    return *m_vertexBuffers[frameIndex];
}

vk::Buffer& MeshRenderer::get_index_buffer(vk::RenderContext& context)
{
    m_indexBuffers.resize(context.get_swapchain_properties().imageCount);
    m_staleIndexBuffers.resize(context.get_swapchain_properties().imageCount, true);

    uint32_t frameIndex = context.get_active_frame_index();
    uint32_t prevFrameIndex = frameIndex == 0
        ? context.get_swapchain_properties().imageCount - 1
        : frameIndex - 1;

    if( m_staleIndexBuffers[frameIndex] )
    {
        if( !m_staleIndexBuffers[prevFrameIndex]
            && m_indexBuffers[prevFrameIndex] )
        {
            m_indexBuffers[frameIndex] = m_indexBuffers[prevFrameIndex];
            m_staleIndexBuffers[frameIndex] = false;
        }
        else
        {
            recreate_index_buffer(context.get_device(), m_indexBuffers[frameIndex], m_mesh->get_indices_size());
            for( int i = 0; i < m_staleIndexBuffers.size(); i++ )
            {
                m_staleIndexBuffers[i] = true;
            }
            m_staleIndexBuffers[frameIndex] = false;
            
            uint8_t* pData = m_indexBuffers[frameIndex]->map();
            memcpy(pData, m_mesh->get_indices().data(), m_mesh->get_indices_size());
            m_indexBuffers[frameIndex]->unmap();
        }
    }
    else
    {
        if( m_mesh->get_index_dirty() )
        {
            recreate_index_buffer(context.get_device(), m_indexBuffers[frameIndex], m_mesh->get_indices_size());
            for( int i = 0; i < m_staleIndexBuffers.size(); i++ )
            {
                m_staleIndexBuffers[i] = true;
            }
            m_staleIndexBuffers[frameIndex] = false;

            uint8_t* pData = m_indexBuffers[frameIndex]->map();
            memcpy(pData, m_mesh->get_indices().data(), m_mesh->get_indices_size());
            m_indexBuffers[frameIndex]->unmap();
        }

    }

    return *m_indexBuffers[frameIndex];
}

void MeshRenderer::recreate_buffer(vk::Device& device,
                                   std::shared_ptr<vk::Buffer>& buffer,
                                   VkDeviceSize size,
                                   VkBufferUsageFlags usage,
                                   VmaMemoryUsage memoryUsage)
{
    if( buffer
     && buffer->get_size() >= size )
    {
        return;
    }

    buffer = std::make_shared<vk::Buffer>(device, size, usage, memoryUsage);
}

void MeshRenderer::recreate_vertex_buffer(vk::Device& device,
                                          std::shared_ptr<vk::Buffer>& buffer,
                                          VkDeviceSize size,
                                          VmaMemoryUsage memoryUsage)
{
    recreate_buffer(device, buffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, memoryUsage);
}

void MeshRenderer::recreate_index_buffer(vk::Device& device,
    std::shared_ptr<vk::Buffer>& buffer,
    VkDeviceSize size,
    VmaMemoryUsage memoryUsage)
{
    recreate_buffer(device, buffer, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, memoryUsage);
}