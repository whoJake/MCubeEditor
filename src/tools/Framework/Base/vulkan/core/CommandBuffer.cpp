#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Device.h"

namespace vk
{

CommandBuffer::CommandBuffer(CommandPool& commandPool, VkCommandBufferLevel level) :
    Resource(VK_NULL_HANDLE, commandPool.get_device()),
    m_commandPool(commandPool),
    m_level(level)
{
    VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = commandPool.get_handle();
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    VkResult allocResult = vkAllocateCommandBuffers(get_device().get_handle(), &allocInfo, &m_handle);
    VK_CHECK(allocResult, "Failed to allocate CommandBuffer.");
}

CommandBuffer::~CommandBuffer()
{
    vkFreeCommandBuffers(get_device().get_handle(), m_commandPool.get_handle(), 1, &m_handle);
}

VkResult CommandBuffer::reset(ResetMode mode)
{
    if( mode == ResetMode::ResetIndividual )
    {
        return vkResetCommandBuffer(get_handle(), 0);
    }
    else
    {
        return VK_SUCCESS;
    }
}

VkCommandBufferLevel CommandBuffer::get_level() const
{
    return m_level;
}

} // vk