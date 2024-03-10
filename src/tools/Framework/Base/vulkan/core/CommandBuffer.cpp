#include "CommandBuffer.h"

#include "CommandPool.h"
#include "Device.h"
#include "RenderPass.h"
#include "vulkan/rendering/RenderTarget.h"
#include "Framebuffer.h"
#include "DescriptorSetLayout.h"
#include "Pipeline.h"
#include "Buffer.h"

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

void CommandBuffer::bind_pipeline(Pipeline& pipeline, VkPipelineBindPoint bindPoint)
{
    vkCmdBindPipeline(get_handle(), bindPoint, pipeline.get_handle());
}

void CommandBuffer::set_viewport(VkViewport viewport)
{
    vkCmdSetViewport(get_handle(), 0, 1, &viewport);
}

void CommandBuffer::set_scissor(VkRect2D scissor)
{
    vkCmdSetScissor(get_handle(), 0, 1, &scissor);
}

void CommandBuffer::push_constants(PipelineLayout& layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pData)
{
    vkCmdPushConstants(get_handle(), layout.get_handle(), stageFlags, offset, size, pData);
}

void CommandBuffer::bind_vertex_buffers(Buffer& buffer, uint32_t binding)
{
    bind_vertex_buffers({ &buffer }, binding, 1);
}

void CommandBuffer::bind_index_buffer(Buffer& buffer, VkIndexType indexType)
{
    vkCmdBindIndexBuffer(get_handle(), buffer.get_handle(), 0, indexType);
}

void CommandBuffer::bind_vertex_buffers(const std::vector<Buffer*>& buffers, uint32_t firstBinding, uint32_t bindingCount)
{
    std::vector<VkBuffer> handles{ };
    std::vector<VkDeviceSize> offsets{ };
    for( auto& buffer : buffers )
    {
        offsets.push_back(0);
        handles.push_back(buffer->get_handle());
    }

    vkCmdBindVertexBuffers(get_handle(), firstBinding, bindingCount, handles.data(), offsets.data());
}

void CommandBuffer::draw_indexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
{
    vkCmdDrawIndexed(get_handle(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(get_handle(), vertexCount, instanceCount, firstVertex, firstInstance);
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