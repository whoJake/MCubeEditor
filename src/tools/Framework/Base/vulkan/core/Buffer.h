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
private:
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usage;
    VmaAllocation m_allocation;
};

} // vk