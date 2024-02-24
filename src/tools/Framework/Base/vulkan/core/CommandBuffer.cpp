#include "CommandBuffer.h"

#include "CommandPool.h"
#include "Device.h"
#include "RenderPass.h"
#include "vulkan/rendering/RenderTarget.h"
#include "Framebuffer.h"
#include "DescriptorSetLayout.h"

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

VkResult CommandBuffer::begin(VkCommandBufferUsageFlags usage, const RenderPass* renderPass, const Framebuffer* framebuffer, uint32_t subpassIndex)
{
    m_state.reset();
    m_descriptorSetLayoutBindingState.clear();

    VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = usage;
    
    // TODO: Handle secondary buffer here

    return vkBeginCommandBuffer(get_handle(), &beginInfo);
}

VkResult CommandBuffer::end()
{
    return vkEndCommandBuffer(get_handle());
}

void CommandBuffer::begin_render_pass(const RenderTarget*              renderTarget,
                                      const RenderPass&                renderPass,
                                      const Framebuffer&               framebuffer,
                                      const std::vector<VkClearValue>& clearValues,
                                      VkSubpassContents                contents)
{
    m_currentRenderPass.renderPass = &renderPass;
    m_currentRenderPass.framebuffer = &framebuffer;

    VkRenderPassBeginInfo beginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
    beginInfo.renderPass = renderPass.get_handle();
    beginInfo.framebuffer = framebuffer.get_handle();
    beginInfo.renderArea.extent = framebuffer.get_extent();
    beginInfo.clearValueCount = to_u32(clearValues.size());
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(get_handle(), &beginInfo, contents);

    ColorBlendState blendState = m_state.get_color_blend_state();
    blendState.attachments.resize(m_currentRenderPass.renderPass->get_color_output_count(m_state.get_subpass_index()));
    m_state.set_color_blend_state(blendState);
}

void CommandBuffer::end_render_pass()
{
    vkCmdEndRenderPass(get_handle());
}

void CommandBuffer::bind_pipeline_layout(PipelineLayout& layout)
{
    m_state.set_pipeline_layout(layout);
}

void CommandBuffer::image_pipeline_barrier(const ImageView&   imageView,
                                           ImageMemoryBarrier memoryBarrier)
{
    VkImageMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    barrier.oldLayout = memoryBarrier.oldLayout;
    barrier.newLayout = memoryBarrier.newLayout;

    barrier.srcAccessMask = memoryBarrier.srcAccessMask;
    barrier.dstAccessMask = memoryBarrier.dstAccessMask;

    barrier.srcQueueFamilyIndex = memoryBarrier.srcQueueFamilyIndex;
    barrier.dstQueueFamilyIndex = memoryBarrier.dstQueueFamilyIndex;

    barrier.image = imageView.get_image().get_handle();
    barrier.subresourceRange = imageView.get_subresource_range();

    vkCmdPipelineBarrier(get_handle(), memoryBarrier.srcStageMask, memoryBarrier.dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}


} // vk