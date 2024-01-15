#include "RenderFrame.h"

namespace vk
{

RenderFrame::RenderFrame(Device&                        device,
                        std::unique_ptr<RenderTarget>&& renderTarget,
                        size_t                          threadCount) :
    m_device(device),
    m_threadCount(threadCount),
    m_fencePool(device),
    m_semaphorePool(device),
    m_swapchainRenderTarget(std::move(renderTarget))
{

}

void RenderFrame::reset()
{
}

Device& RenderFrame::get_device() const
{
    return m_device;
}

RenderTarget& RenderFrame::get_render_target()
{
    return *m_swapchainRenderTarget;
}

const RenderTarget& RenderFrame::get_render_target_const() const
{
    return *m_swapchainRenderTarget;
}

const FencePool& RenderFrame::get_fence_pool() const
{
    return m_fencePool;
}

VkFence RenderFrame::request_fence()
{
    return m_fencePool.request_fence();
}

const SemaphorePool& RenderFrame::get_semaphore_pool() const
{
    return m_semaphorePool;
}

VkSemaphore RenderFrame::request_semaphore()
{
    return m_semaphorePool.request_semaphore();
}

VkSemaphore RenderFrame::request_semaphore_with_ownership()
{
    return m_semaphorePool.request_semaphore_with_ownership();
}

void RenderFrame::release_owned_semaphore(VkSemaphore semaphore)
{
    m_semaphorePool.release_owned_semaphore(semaphore);
}

void RenderFrame::clear_descriptors()
{
}

void RenderFrame::update_descriptor_sets(size_t threadIndex)
{
}

void RenderFrame::update_render_target(std::unique_ptr<RenderTarget>&& renderTarget)
{
}

} // vk