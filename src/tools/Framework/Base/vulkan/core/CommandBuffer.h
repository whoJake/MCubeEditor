#pragma once

#include "vulkan/common.h"
#include "Resource.h"

namespace vk
{

class Device;
class CommandPool;
class RenderTarget;
class RenderPass;
class Framebuffer;
class PipelineState;
class PipelineLayout;

class CommandBuffer : public Resource<VkCommandBuffer>
{
public:
    enum class ResetMode
    {
        ResetPool,
        ResetIndividual,
        AlwaysAllocate,
    };

    struct RenderPassBinding
    {
        const RenderPass* renderPass;
        const Framebuffer* framebuffer;
    };

    CommandBuffer(CommandPool& commandPool, VkCommandBufferLevel level);
    ~CommandBuffer();

    VkResult reset(ResetMode resetMode);

    VkCommandBufferLevel get_level() const;

    VkResult begin(VkCommandBufferUsageFlags usage, const RenderPass* renderPass, const Framebuffer* framebuffer, uint32_t subpassIndex);

    VkResult end();

    void begin_render_pass(const RenderTarget*              renderTarget,
                           const RenderPass&                renderPass,
                           const Framebuffer&               framebuffer,
                           const std::vector<VkClearValue>& clearValues,
                           VkSubpassContents                contents = VK_SUBPASS_CONTENTS_INLINE);

    void end_render_pass();

    void bind_pipeline_layout(PipelineLayout& layout);
private:
    CommandPool& m_commandPool;
    VkCommandBufferLevel m_level;

    RenderPassBinding m_currentRenderPass;
    PipelineState m_state;
    std::unordered_map<uint32_t, DescriptorSetLayout*> m_descriptorSetLayoutBindingState;
};

} // vk