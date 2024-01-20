#pragma once

#include "vulkan/common.h"
#include "Resource.h"

namespace vk
{

class Device;
class CommandPool;

class CommandBuffer : public Resource<VkCommandBuffer>
{
public:
    enum class ResetMode
    {
        ResetPool,
        ResetIndividual,
        AlwaysAllocate,
    };

    CommandBuffer(CommandPool& commandPool, VkCommandBufferLevel level);
    ~CommandBuffer();

    VkResult reset(ResetMode resetMode);

    VkCommandBufferLevel get_level() const;
private:
    CommandPool& m_commandPool;

    VkCommandBufferLevel m_level;
};

} // vk