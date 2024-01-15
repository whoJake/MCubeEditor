#pragma once

#include "vulkan/common.h"
#include "ShaderModule.h"
#include "Resource.h"

namespace vk
{

class Device;

class PipelineLayout : Resource<VkPipelineLayout>
{
public:
    PipelineLayout(Device& device, const std::vector<ShaderModule*>& shaderModules);
    ~PipelineLayout() override;

    PipelineLayout(const PipelineLayout&) = delete;
    PipelineLayout(PipelineLayout&&);
    PipelineLayout& operator=(const PipelineLayout&) = delete;
    PipelineLayout& operator=(PipelineLayout&&) = delete;

    const std::vector<ShaderModule*>& get_shader_modules() const;

    const std::vector<ShaderResource> get_resources(const ShaderResourceType& type = ShaderResourceType::All, VkShaderStageFlags stages = VK_SHADER_STAGE_ALL) const;

private:
    std::vector<ShaderModule*> m_shaderModules;

    std::unordered_map<std::string, ShaderResource> m_shaderResources;
};

} // vk