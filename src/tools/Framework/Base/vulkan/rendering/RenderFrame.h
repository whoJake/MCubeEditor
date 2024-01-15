#pragma once

#include "vulkan/common.h"
#include "RenderTarget.h"
#include "vulkan/core/Device.h"
#include "vulkan/core/FencePool.h"
#include "vulkan/core/SemaphorePool.h"

namespace vk
{

class RenderFrame
{
public:
    RenderFrame(Device&                         device,
                std::unique_ptr<RenderTarget>&& renderTarget,
                size_t                          threadCount   = 1);

    RenderFrame(const RenderFrame&) = delete;
    RenderFrame(RenderFrame&&) = delete;
    RenderFrame& operator=(const RenderFrame&) = delete;
    RenderFrame& operator=(RenderFrame&&) = delete;

    void reset();

    Device& get_device() const;

    RenderTarget& get_render_target();

    const RenderTarget& get_render_target_const() const;

    const FencePool& get_fence_pool() const;

    VkFence request_fence();

    const SemaphorePool& get_semaphore_pool() const;

    VkSemaphore request_semaphore();

    VkSemaphore request_semaphore_with_ownership();

    void release_owned_semaphore(VkSemaphore semaphore);

    // CommandBuffer& request_command_buffer(const Queue& queue, CommandBuffer::ResetMode resetMode, VkCommandBufferLevel level, size_t threadIndex);

    // VkDescriptorSet request_descriptor_set(??);

    void clear_descriptors();

    // allocate_buffer?

    void update_descriptor_sets(size_t threadIndex = 0);

    void update_render_target(std::unique_ptr<RenderTarget>&& renderTarget);
private:
    Device& m_device;

    size_t m_threadCount;

    FencePool m_fencePool;
    SemaphorePool m_semaphorePool;

    std::unique_ptr<RenderTarget> m_swapchainRenderTarget;
};

} // vk