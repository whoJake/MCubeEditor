#include "Buffer.h"

#include "Device.h"

namespace vk
{

Buffer::Buffer(Device&            device,
               VkDeviceSize       size,
               VkBufferUsageFlags usage,
               VmaMemoryUsage     memoryUsage) :
    Resource(VK_NULL_HANDLE, device),
    m_size(size),
    m_usage(usage),
    m_allocation(nullptr)
{
    VkBufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    createInfo.size = size;
    createInfo.usage = usage;

    VmaAllocationCreateInfo allocInfo{ };
    allocInfo.usage = memoryUsage;

    VkResult result = vmaCreateBuffer(get_device().get_allocator(), &createInfo, &allocInfo, &m_handle, &m_allocation, nullptr);
    VK_CHECK(result, "Failed to create Buffer.");
}

Buffer::~Buffer() {
    vmaDestroyBuffer(get_device().get_allocator(), m_handle, m_allocation);
}

VkDeviceSize Buffer::get_size() const {
    return m_size;
}

VkBufferUsageFlags Buffer::get_usage() const {
    return m_usage;
}

const VmaAllocation& Buffer::get_allocation() const {
    return m_allocation;
}

} // vk