#include "FixedChunk.h"
#include "vulkan/core/Device.h"

FixedChunk::FixedChunk(vk::Device& device, const glm::vec3 origin, const glm::uvec3 dimensions) :
    m_dimensions(dimensions),
    m_origin(origin),
    m_dirty(true)
{
    // Debug cube at origin
    m_data.vertices.push_back({ -1.f, -1.f, -1.f });
    m_data.vertices.push_back({ 1.f, -1.f, -1.f });
    m_data.vertices.push_back({ -1.f, 1.f, -1.f });
    m_data.vertices.push_back({ 1.f, 1.f, -1.f });
    m_data.vertices.push_back({ -1.f, -1.f, 1.f });
    m_data.vertices.push_back({ 1.f, -1.f, 1.f });
    m_data.vertices.push_back({ -1.f, 1.f, 1.f });
    m_data.vertices.push_back({ 1.f, 1.f, 1.f });

    //back
    m_data.indices.push_back(0),m_data.indices.push_back(2),m_data.indices.push_back(1);
    m_data.indices.push_back(3),m_data.indices.push_back(1),m_data.indices.push_back(2);

    //front
    m_data.indices.push_back(5),m_data.indices.push_back(7),m_data.indices.push_back(4);
    m_data.indices.push_back(6),m_data.indices.push_back(4),m_data.indices.push_back(7);

    //left
    m_data.indices.push_back(4),m_data.indices.push_back(6),m_data.indices.push_back(0);
    m_data.indices.push_back(2),m_data.indices.push_back(0),m_data.indices.push_back(6);

    //right
    m_data.indices.push_back(1),m_data.indices.push_back(3),m_data.indices.push_back(5);
    m_data.indices.push_back(7),m_data.indices.push_back(5),m_data.indices.push_back(3);

    //top
    m_data.indices.push_back(7),m_data.indices.push_back(3),m_data.indices.push_back(6);
    m_data.indices.push_back(2),m_data.indices.push_back(6),m_data.indices.push_back(3);

    //bottom
    m_data.indices.push_back(1),m_data.indices.push_back(5),m_data.indices.push_back(0);
    m_data.indices.push_back(4),m_data.indices.push_back(0),m_data.indices.push_back(5);

    m_vertexBuffer = std::make_unique<vk::Buffer>(device, sizeof(glm::vec3) * m_data.vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
    m_indexBuffer = std::make_unique<vk::Buffer>(device, sizeof(uint32_t) * m_data.indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
}

FixedChunk::~FixedChunk()
{ }

const FixedChunk::Data& FixedChunk::get_data() const
{
    return m_data;
}

size_t FixedChunk::get_triangle_count() const
{
    return m_data.indices.size() / 3;
}

const glm::uvec3& FixedChunk::get_dimensions() const
{
    return m_dimensions;
}

vk::Buffer& FixedChunk::get_vertex_buffer()
{
    update_buffer_mapping();
    return *m_vertexBuffer;
}

vk::Buffer& FixedChunk::get_index_buffer()
{
    update_buffer_mapping();
    return *m_indexBuffer;
}

void FixedChunk::update_buffer_mapping()
{
    if( !m_dirty )
    {
        return;
    }

    uint8_t* pVertexData = m_vertexBuffer->map();
    uint8_t* pIndexData = m_indexBuffer->map();

    memcpy(pVertexData, m_data.vertices.data(), sizeof(glm::vec3) * m_data.vertices.size());
    memcpy(pIndexData, m_data.indices.data(), sizeof(uint32_t) * m_data.indices.size());

    m_vertexBuffer->unmap();
    m_indexBuffer->unmap();
}