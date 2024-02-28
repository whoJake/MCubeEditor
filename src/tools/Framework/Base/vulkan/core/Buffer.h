#pragma once

#include "vulkan/common.h"
#include "Resource.h"

namespace vk
{

class Buffer : public Resource<VkBuffer>
{
public:
    Buffer(Device&            device,
           VkDeviceSize       size,
           VkBufferUsageFlags usage,
           VmaMemoryUsage     memoryUsage);
    ~Buffer() override;

    VkDeviceSize get_size() const;

    VkBufferUsageFlags get_usage() const;

    const VmaAllocation& get_allocation() const;

    uint8_t* map();

    void unmap();
private:
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usage;
    VmaAllocation m_allocation;

    uint8_t* m_mappedData{ nullptr };
    bool m_mapped{ false };
};

} // vk