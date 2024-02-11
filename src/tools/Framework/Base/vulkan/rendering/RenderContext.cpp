#include "RenderContext.h"

#include "vulkan/core/Device.h"
#include "vulkan/core/Image.h"
#include "RenderTarget.h"

namespace vk
{

RenderContext::RenderContext(Device&                                device,
                             VkSurfaceKHR                           surface,
                             const Window&                          window,
                             const std::vector<VkPresentModeKHR>&   presentModePriorityList,
                             const std::vector<VkSurfaceFormatKHR>& surfaceFormatPriorityList,
                             RenderTarget::CreateFunc               renderTargetCreateFunc,
                             size_t                                 threadCount) :
    m_device(device),
    m_window(window),
    m_surfaceExtent({ window.get_extent().width, window.get_extent().height }),
    m_threadCount(threadCount),
    m_renderTargetCreateFunc(renderTargetCreateFunc)
{
    if( surface != VK_NULL_HANDLE )
    {
        VkSurfaceCapabilitiesKHR capabilities{ };
        VkResult capabilitiesResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.get_gpu().get_handle(), surface, &capabilities);
        VK_CHECK(capabilitiesResult, "Failed to get physical device surface capabilities.");
        
        // Anything supported
        if( capabilities.currentExtent.width == 0xFFFFFFFF )
        {
            m_swapchain = std::make_unique<Swapchain>(m_device, surface, presentModePriorityList, surfaceFormatPriorityList, m_surfaceExtent);
        }
        else
        {
            m_swapchain = std::make_unique<Swapchain>(m_device, surface, presentModePriorityList, surfaceFormatPriorityList);
        }

        m_surfaceExtent = m_swapchain->get_extent();
        VkExtent3D imageExtents = { m_surfaceExtent.width, m_surfaceExtent.height, 1 };

        for( auto& image : m_swapchain->get_images() )
        {
            Image colorImage(m_device,
                             imageExtents,
                             m_swapchain->get_format(),
                             VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                             VMA_MEMORY_USAGE_GPU_ONLY);

            // Need lambda here to avoid using any Image& and also gives option to redefine createfunc
            // if don't need depth/stencil or smth else.
            std::unique_ptr<RenderTarget> renderTarget = m_renderTargetCreateFunc(std::move(colorImage));
            m_frames.emplace_back(std::make_unique<RenderFrame>(m_device, std::move(renderTarget), m_threadCount));
        }
    }
    else
    {
        m_swapchain = nullptr;
        throw VulkanException("Headless rendercontext isn't supported yet.");
    }
}

VkSemaphore RenderContext::submit(const Queue&                             queue,
                                  CommandBuffer&                           commandBuffer,
                                  const std::vector<VkSemaphore>&          waitSemaphores,
                                  const std::vector<VkPipelineStageFlags>& waitStageFlags)
{
    return submit(queue, { &commandBuffer }, waitSemaphores, waitStageFlags);
}

VkSemaphore RenderContext::submit(const Queue&                             queue,
                                  const std::vector<CommandBuffer*>&       commandBuffers,
                                  const std::vector<VkSemaphore>&          waitSemaphores,
                                  const std::vector<VkPipelineStageFlags>& waitStageFlags)
{
    std::vector<VkCommandBuffer> bufferHandles;
    for( const CommandBuffer* buffer : commandBuffers )
    {
        bufferHandles.push_back( buffer->get_handle() );
    }

    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = to_u32(commandBuffers.size());
    submitInfo.pCommandBuffers = bufferHandles.data();
    submitInfo.waitSemaphoreCount = to_u32(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStageFlags.data();

    VkSemaphore signalSemaphore = get_active_frame().request_semaphore();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore;

    VkResult submitResult = queue.submit({ submitInfo }, get_active_frame().request_fence());
    VK_CHECK(submitResult, "Failed to submit command buffers.");

    return signalSemaphore;
}

void RenderContext::begin_frame()
{
    if( m_swapchain )
    {
        handle_surface_changes();
    }

    VK_ASSERT(!m_frameActive, "The frame is still active so cannot begin a new one. Make sure end_frame has been called.");
    auto& previousFrame = *m_frames[m_activeFrameIndex];

    m_aquiredSemaphore = previousFrame.request_semaphore_with_ownership();

    if( m_swapchain )
    {
        VkResult indexResult = m_swapchain->acquire_next_image(&m_activeFrameIndex, m_aquiredSemaphore, VK_NULL_HANDLE);

        if( indexResult == VK_SUBOPTIMAL_KHR || indexResult == VK_ERROR_OUT_OF_DATE_KHR )
        {
            bool swapchainUpdated = handle_surface_changes(indexResult == VK_ERROR_OUT_OF_DATE_KHR);

            if( swapchainUpdated )
            {
                indexResult = m_swapchain->acquire_next_image(&m_activeFrameIndex, m_aquiredSemaphore, VK_NULL_HANDLE);
            }
        }

        if( indexResult != VK_SUCCESS )
        {
            previousFrame.reset();
            return;
        }
    }

    m_frameActive = true;

    wait_frame();
}

void RenderContext::end_frame(VkSemaphore semaphore)
{
    VK_ASSERT(m_frameActive, "The frame is not active so cannot end frame. Make sure begin_frame has been called.");

    if( m_swapchain )
    {
        VkSwapchainKHR swapchain = m_swapchain->get_handle();
        VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &semaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &m_activeFrameIndex;

        VkResult result = m_device.get_queue_by_present(0).present(presentInfo);

        if( result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR )
        {
            handle_surface_changes();
        }
    }

    if( !m_aquiredSemaphore )
    {
        get_active_frame().release_owned_semaphore(m_aquiredSemaphore);
        m_aquiredSemaphore = VK_NULL_HANDLE;
    }
    m_frameActive = false;
}

void RenderContext::wait_frame()
{
    get_active_frame().reset();
}

RenderFrame& RenderContext::get_active_frame()
{
    VK_ASSERT(m_frameActive, "There is no active frame, make sure that begin_frame has been called.");
    return *m_frames[m_activeFrameIndex];
}

uint32_t RenderContext::get_active_frame_index() const
{
    VK_ASSERT(m_frameActive, "There is no active frame, make sure that begin_frame has been called.");
    return m_activeFrameIndex;
}

bool RenderContext::handle_surface_changes(bool forceUpdate)
{
    if( !m_swapchain )
    {
        // Nothing to handle.
        return false;
    }

    VkSurfaceCapabilitiesKHR capabilities{ };
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.get_gpu().get_handle(), m_swapchain->get_surface(), &capabilities);
    VK_CHECK(result, "Failed to get physical device surface capabilities.");

    if( capabilities.currentExtent.width = 0xFFFFFFFF )
    {
        // Swapchain makes the plays, nothing needs to happen
        return false;
    }

    if( capabilities.currentExtent.width != m_surfaceExtent.width
        || capabilities.currentExtent.height != m_surfaceExtent.height
        || forceUpdate)
    {
        m_device.wait_idle();
        update_swapchain(capabilities.currentExtent);

        m_surfaceExtent = capabilities.currentExtent;
        return true;
    }

    return false;
}

void RenderContext::update_swapchain(VkExtent2D extent)
{
    if( !m_swapchain )
    {
        // No swapchain to update ;3
        return;
    }

    // These will all be invalid so need to be regenerated next time around.
    m_device.get_resource_cache().clear_framebuffers();

    uint32_t width = extent.width;
    uint32_t height = extent.height;

    m_swapchain = std::make_unique<Swapchain>(*m_swapchain, extent);

    recreate();
}

void RenderContext::recreate()
{
    VkExtent2D swapchainExtent = m_swapchain->get_extent();
    VkExtent3D extent{ swapchainExtent.width, swapchainExtent.height, 1 };

    auto it = m_frames.begin();

    // Recreate all images in swapchain
    for( auto& handle : m_swapchain->get_images() )
    {
        Image swapchainImage(m_device,
                             handle,
                             extent,
                             m_swapchain->get_format(),
                             m_swapchain->get_usage());

        std::unique_ptr<RenderTarget> renderTarget = m_renderTargetCreateFunc(std::move(swapchainImage));
        if( it != m_frames.end() )
        {
            (*it)->update_render_target(std::move(renderTarget));
        }
        else
        {
            // Creates new image if new swapchain has more images than the last one.
            m_frames.emplace_back(std::make_unique<RenderFrame>(m_device, std::move(renderTarget), m_threadCount));
        }

        ++it;
    }

    m_device.get_resource_cache().clear_framebuffers();
}

Device& RenderContext::get_device()
{
    return m_device;
}

} // vk