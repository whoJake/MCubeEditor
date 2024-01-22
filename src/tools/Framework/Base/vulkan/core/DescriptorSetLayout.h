#pragma once

#include "vulkan/common.h"
#include "Resource.h"

namespace vk
{

class Device;
class ShaderModule;
class ShaderResource;

class DescriptorSetLayout : public Resource<VkDescriptorSetLayout>
{
public:
    DescriptorSetLayout(Device&                            device,
                        uint32_t                           setIndex,
                        const std::vector<ShaderModule*>&  shaderModules,
                        const std::vector<ShaderResource>& resources);

    ~DescriptorSetLayout();

    DescriptorSetLayout(DescriptorSetLayout&&) = delete;
    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;
    DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

    uint32_t get_index() const;

    const std::vector<VkDescriptorSetLayoutBinding>& get_bindings() const;

    const std::vector<ShaderModule*>& get_shader_modules() const;
private:
    uint32_t m_setIndex;
    std::vector<VkDescriptorSetLayoutBinding> m_bindings;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindingMap;
    std::unordered_map<std::string, uint32_t> m_resourceToBindingMap;

    std::vector<ShaderModule*> m_shaderModules;

};

} // vk