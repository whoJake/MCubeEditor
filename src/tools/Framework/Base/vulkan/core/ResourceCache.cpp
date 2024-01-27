#include "ResourceCache.h"
#include "Device.h"
#include "common/hashers.h"

namespace vk
{

/*
 * IF THERE ARE COMPILE ERRORS EG.
 * CG2660 'std::pair<a, b>::pair': function does not take 2 arguments.
 * THEN ITS LIKELY A MISSING COPY CONSTRUCTOR ON ONE OF THE RESOURCES ( Resource::Resource(Resource&&) )
 */

template<typename T, typename... A>
T& request_resource(const jclog::Log& log, std::unordered_map<size_t, T>& resources, A&... args)
{
    size_t hash{ 0 };
    hash_params(hash, args...);

    auto it = resources.find(hash);
    if( it != resources.end() )
        return (*it).second;

    // Could be reduced to a single .emplace call but its useful to know when an object is being created before creating it

    JCLOG_INFO(log, "Generating #{} ( {} ) cache object", resources.size(), typeid(T).name());
    T resource(args...);
    auto ins_it = resources.emplace(hash, std::move(resource));
    return (ins_it.first)->second;
}

ResourceCache::ResourceCache(Device& device) :
    m_device(device),
    m_state()
{ }

ShaderModule& ResourceCache::request_shader_module(VkShaderStageFlagBits stage, const std::vector<uint8_t>& glslSource, const std::string& entryPoint)
{
    return request_resource(m_device.get_log(),  m_state.shaderModules, m_device, stage, glslSource, entryPoint);
}

Framebuffer& ResourceCache::request_framebuffer(const RenderTarget& renderTarget, const RenderPass& renderPass)
{
    return request_resource(m_device.get_log(), m_state.framebuffers, m_device, renderTarget, renderPass);
}

DescriptorSetLayout& ResourceCache::request_descriptor_set_layout(uint32_t setIndex, const std::vector<ShaderModule*>& shaderModules, const std::vector<ShaderResource>& resources)
{
    return request_resource(m_device.get_log(), m_state.descriptorSetLayouts, m_device, setIndex, shaderModules, resources);
}

void ResourceCache::clear_framebuffers()
{
    m_state.framebuffers.clear();
}

void ResourceCache::clear()
{
    m_state.shaderModules.clear();
    m_state.descriptorSetLayouts.clear();
}

const ResourceCacheState& ResourceCache::get_internal_state() const
{
    return m_state;
}

} // vk