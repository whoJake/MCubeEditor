#include "Pipeline.h"

#include "Device.h"

namespace vk
{

Pipeline::Pipeline(Device& device) :
    Resource(VK_NULL_HANDLE, device)
{ }

Pipeline::Pipeline(Pipeline&& other) :
    Resource(other.m_handle, other.m_device),
    m_state(other.m_state)
{
    other.m_handle = VK_NULL_HANDLE;
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(get_device().get_handle(), m_handle, nullptr);
}

const PipelineState& Pipeline::get_state() const
{
    return m_state;
}

GraphicsPipeline::GraphicsPipeline(Device&         device,
                                   VkPipelineCache cache,
                                   PipelineState&  state) :
    Pipeline(device)
{


}

} // vk