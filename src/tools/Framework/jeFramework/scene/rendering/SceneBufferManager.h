#pragma once

#include "scene/rendering/proxies/BlueprintProxy.h"
#include "scene/gameplay/Mesh.h"
#include "core/Buffer.h"
#include "rendering/RenderContext.h"

struct BlueprintBuffers
{
    std::vector<std::shared_ptr<vk::Buffer>> vertex{ };
    std::shared_ptr<vk::Buffer> index{ };
};

enum class BufferLoadingStrategy
{
    Dynamic, // Keep in cpu memory. Changed often.
    Static   // Upload to device local memory. Not changed often
};

class SceneBufferManager
{
private:
    struct BufferIntegrity
    {
        std::vector<bool> vertex{ };
        bool index{ };
    };

    struct BufferData
    {
        BlueprintBuffers buffers{ };
        BufferIntegrity integrities{ };
    };
public:
    SceneBufferManager(vk::RenderContext& renderContext);
    ~SceneBufferManager();

    BlueprintBuffers& get_mesh_buffer_data(const BlueprintProxy& blueprint, BufferLoadingStrategy strategy = BufferLoadingStrategy::Dynamic);
private:
    std::vector<BufferData>& find_or_create_blueprint_buffer_data(bpid_t id, uint32_t vertexBufferCount);
    void create_buffer(std::shared_ptr<vk::Buffer>& existingBuffer, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
    void create_vertex_buffer(std::shared_ptr<vk::Buffer>& existingBuffer, VkDeviceSize size, VmaMemoryUsage memoryUsage);
    void create_index_buffer(std::shared_ptr<vk::Buffer>& existingBuffer, VkDeviceSize size, VmaMemoryUsage memoryUsage);

    void invalidate_vertex_buffers(std::vector<BufferData>& bufferDatas);
    void invalidate_index_buffers(std::vector<BufferData>& bufferDatas);

    void direct_map_buffer(vk::Buffer& buffer, const void* data, size_t size);
private:
    vk::RenderContext& m_renderContext;
    std::unordered_map<bpid_t, std::vector<BufferData>> m_bufferMappings;
};