#pragma once

#include "vulkan/common.h"
#include "ShaderModule.h"
#include "Framebuffer.h"

namespace vk
{

class Device;

struct ResourceCacheState
{
    std::unordered_map<size_t, ShaderModule> shaderModules;
    std::unordered_map<size_t, Framebuffer> framebuffers;
};

class ResourceCache
{
public:
    ResourceCache(Device& device);

    ResourceCache(const ResourceCache& other) = delete;
    ResourceCache(ResourceCache&& other) = delete;
    ResourceCache& operator=(const ResourceCache& other) = delete;
    ResourceCache& operator=(ResourceCache&& other) = delete;

    ShaderModule& request_shader_module(VkShaderStageFlagBits stage, const std::vector<uint8_t>& glslSource, const std::string& entryPoint);

    Framebuffer& request_framebuffer(const RenderTarget& renderTarget, const RenderPass& renderPass);

    void clear_framebuffers();

    void clear();

    const ResourceCacheState& get_internal_state() const;

private:
    Device& m_device;

    ResourceCacheState m_state;

};

} // vk