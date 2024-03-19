#include "SceneBufferManager.h"

#include <iostream>

SceneBufferManager::SceneBufferManager(vk::RenderContext& renderContext) :
    m_renderContext(renderContext)
{ }

SceneBufferManager::~SceneBufferManager()
{ }

BlueprintBuffers& SceneBufferManager::get_mesh_buffer_data(const BlueprintProxy& blueprint, BufferLoadingStrategy strategy)
{
    std::vector<BufferData>& bufferDatas = find_or_create_blueprint_buffer_data(blueprint.get_id(), blueprint.get_mesh().get_vertex_buffer_count());

    uint32_t frameIndex = m_renderContext.get_active_frame_index();
    uint32_t prevFrameIndex = (frameIndex - 1) % m_renderContext.get_swapchain_properties().imageCount;

    BufferData& frameData = bufferDatas.at(frameIndex);
    BufferData& prevFrameData = bufferDatas.at(prevFrameIndex);

    // Index buffer

    // If we have no integrity, we always need a new buffer (create or inherit)
    if( !frameData.integrities.index )
    {
        if( prevFrameData.integrities.index )
        {
            // inherit when previous buffer has integrity
            frameData.buffers.index = prevFrameData.buffers.index;
            frameData.integrities.index = true;
        }
        else
        {
            // creating new buffer so need to invalidate previous ones
            invalidate_index_buffers(bufferDatas);

            // create
            switch( strategy )
            {
            case BufferLoadingStrategy::Dynamic:
                create_index_buffer(frameData.buffers.index,
                                    blueprint.get_mesh().get_indices_size(),
                                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                direct_map_buffer(*frameData.buffers.index,
                                  blueprint.get_mesh().get_indices().data(),
                                  blueprint.get_mesh().get_indices_size());
                break;
            case BufferLoadingStrategy::Static:
                create_index_buffer(frameData.buffers.index,
                                    blueprint.get_mesh().get_indices_size(),
                                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                direct_map_buffer(*frameData.buffers.index,
                                  blueprint.get_mesh().get_indices().data(),
                                  blueprint.get_mesh().get_indices_size());
                break;
            }
        }
    }
    else
    {
        if( blueprint.get_mesh().get_index_dirty() )
        {
            // creating new buffer so need to invalidate previous ones
            invalidate_index_buffers(bufferDatas);

            // always create on dirty
            switch( strategy )
            {
            case BufferLoadingStrategy::Dynamic:
                create_index_buffer(frameData.buffers.index,
                                    blueprint.get_mesh().get_indices_size(),
                                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                direct_map_buffer(*frameData.buffers.index,
                                  blueprint.get_mesh().get_indices().data(),
                                  blueprint.get_mesh().get_indices_size());
                break;
            case BufferLoadingStrategy::Static:
                create_index_buffer(frameData.buffers.index,
                                    blueprint.get_mesh().get_indices_size(),
                                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                direct_map_buffer(*frameData.buffers.index,
                                  blueprint.get_mesh().get_indices().data(),
                                  blueprint.get_mesh().get_indices_size());
                break;
            }

            // Remove integrity from all other buffers
            for( BufferData& data : bufferDatas )
            {
                data.integrities.index = false;
            }
        }

        // oherwise we gucci or integrities is wrong >.<
    }

    // Vertex buffers
    for( uint32_t bufferIndex = 0; bufferIndex < blueprint.get_mesh().get_vertex_buffer_count(); bufferIndex++ )
    {
        if( !frameData.integrities.vertex.at(bufferIndex) )
        {
            if( prevFrameData.integrities.vertex.at(bufferIndex) )
            {
                // inherit when previous buffer has integrity
                frameData.buffers.vertex.at(bufferIndex) = prevFrameData.buffers.vertex.at(bufferIndex);
                frameData.integrities.vertex.at(bufferIndex) = true;
            }
            else
            {
                // creating new buffer so need to invalidate previous ones
                invalidate_vertex_buffers(bufferDatas);

                // create
                switch( strategy )
                {
                case BufferLoadingStrategy::Dynamic:
                    create_vertex_buffer(frameData.buffers.vertex.at(bufferIndex),
                                        blueprint.get_mesh().get_vertices_size(bufferIndex),
                                        VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                    direct_map_buffer(*frameData.buffers.vertex.at(bufferIndex),
                                      blueprint.get_mesh().get_vertices(bufferIndex).data(),
                                      blueprint.get_mesh().get_vertices_size(bufferIndex));
                    break;
                case BufferLoadingStrategy::Static:
                    create_vertex_buffer(frameData.buffers.vertex.at(bufferIndex),
                                        blueprint.get_mesh().get_vertices_size(bufferIndex),
                                        VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                    direct_map_buffer(*frameData.buffers.vertex.at(bufferIndex),
                                      blueprint.get_mesh().get_vertices(bufferIndex).data(),
                                      blueprint.get_mesh().get_vertices_size(bufferIndex));
                    break;
                }
                
                // Remove integrity from all other buffers
                for( BufferData& data : bufferDatas )
                {
                    for( size_t i = 0; i < data.integrities.vertex.size(); i++ )
                    {
                        data.integrities.vertex.at(i) = false;
                    }
                }
            }
        }
        else
        {
            if( blueprint.get_mesh().get_vertex_dirty(bufferIndex) )
            {
                // creating new buffer so need to invalidate previous ones
                invalidate_vertex_buffers(bufferDatas);

                // create
                switch( strategy )
                {
                case BufferLoadingStrategy::Dynamic:
                    create_vertex_buffer(frameData.buffers.vertex.at(bufferIndex),
                        blueprint.get_mesh().get_vertices_size(bufferIndex),
                        VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                    direct_map_buffer(*frameData.buffers.vertex.at(bufferIndex),
                        blueprint.get_mesh().get_vertices(bufferIndex).data(),
                        blueprint.get_mesh().get_vertices_size(bufferIndex));
                    break;
                case BufferLoadingStrategy::Static:
                    create_vertex_buffer(frameData.buffers.vertex.at(bufferIndex),
                        blueprint.get_mesh().get_vertices_size(bufferIndex),
                        VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
                    direct_map_buffer(*frameData.buffers.vertex.at(bufferIndex),
                        blueprint.get_mesh().get_vertices(bufferIndex).data(),
                        blueprint.get_mesh().get_vertices_size(bufferIndex));
                    break;
                }
            }
        }
    }

    // Ensure current integrity to true
    frameData.integrities.index = true;
    for( size_t i = 0; i < frameData.integrities.vertex.size(); i++ )
    {
        frameData.integrities.vertex.at(i) = true;
    }

    return frameData.buffers;
}

std::vector<SceneBufferManager::BufferData>& SceneBufferManager::find_or_create_blueprint_buffer_data(bpid_t id, uint32_t vertexBufferCount)
{
    auto it = m_bufferMappings.find(id);
    if( it != m_bufferMappings.end() )
    {
        return it->second;
    }

    // Make new one
    std::vector<BufferData>& result = m_bufferMappings.emplace(std::piecewise_construct,
                                                               std::tuple(id),
                                                               std::tuple()).first->second;

    // Add in all vertex buffers
    result.resize(m_renderContext.get_swapchain_properties().imageCount);
    for( auto& buffers : result )
    {
        buffers.buffers.vertex.resize(vertexBufferCount);
        buffers.integrities.vertex.resize(vertexBufferCount);
    }

    return result;
}

void SceneBufferManager::create_buffer(std::shared_ptr<vk::Buffer>& buffer, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
    if( buffer
     && buffer->get_size() >= size )
    {
        return;
    }

    buffer = std::make_shared<vk::Buffer>(m_renderContext.get_device(), size, usage, memoryUsage);
}

void SceneBufferManager::create_vertex_buffer(std::shared_ptr<vk::Buffer>& buffer, VkDeviceSize size, VmaMemoryUsage memoryUsage)
{
    create_buffer(buffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, memoryUsage);
}

void SceneBufferManager::create_index_buffer(std::shared_ptr<vk::Buffer>& buffer, VkDeviceSize size, VmaMemoryUsage memoryUsage)
{
    create_buffer(buffer, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, memoryUsage);
}

void SceneBufferManager::invalidate_vertex_buffers(std::vector<BufferData>& datas)
{
    for( BufferData& data : datas )
    {
        for( size_t i = 0; i < data.integrities.vertex.size(); i++ )
        {
            data.integrities.vertex.at(i) = false;
        }
    }
}

void SceneBufferManager::invalidate_index_buffers(std::vector<BufferData>& datas)
{
    for( BufferData& data : datas )
    {
        data.integrities.index = false;
    }
}

void SceneBufferManager::direct_map_buffer(vk::Buffer& buffer, const void* data, size_t size)
{
    uint8_t* mappedData = buffer.map();
    memcpy(mappedData, data, size);
    buffer.unmap();
}