#pragma once

#include "engine/scene/gameplay/Mesh.h"
#include "vulkan/core/Buffer.h"
#include "vulkan/rendering/RenderContext.h"

class MeshRenderer
{
public:
    MeshRenderer(Mesh* mesh);
    ~MeshRenderer();

    vk::Buffer& get_vertex_buffer(vk::RenderContext& context);
    vk::Buffer& get_index_buffer(vk::RenderContext& context);
private:
    void recreate_buffer(vk::Device& device,
                         std::shared_ptr<vk::Buffer>& buffer,
                         VkDeviceSize size,
                         VkBufferUsageFlags usage,
                         VmaMemoryUsage memoryUsage);

    void recreate_vertex_buffer(vk::Device& device,
                                std::shared_ptr<vk::Buffer>& buffer,
                                VkDeviceSize size,
                                VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST);

    void recreate_index_buffer(vk::Device& device,
                                std::shared_ptr<vk::Buffer>& buffer,
                                VkDeviceSize size,
                                VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST);
private:
    Mesh* m_mesh;

    std::vector<std::shared_ptr<vk::Buffer>> m_vertexBuffers;
    std::vector<bool> m_staleVertexBuffers;
    std::vector<std::shared_ptr<vk::Buffer>> m_indexBuffers;
    std::vector<bool> m_staleIndexBuffers;
};