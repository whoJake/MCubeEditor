#include "DescriptorSet.h"

namespace vk
{

DescriptorSet::DescriptorSet(Device&                             device,
                             DescriptorPool&                     pool,
                             std::vector<VkDescriptorBufferInfo> bufferInfos,
                             std::vector<VkDescriptorImageInfo>  imageInfos) :
    Resource(pool.allocate(), device),
    m_pool(pool),
    m_bufferInfos(bufferInfos),
    m_imageInfos(imageInfos)
{ }

const DescriptorSetLayout& DescriptorSet::get_descriptor_set_layout() const
{
    return m_pool.get_layout();
}

} // vk