#pragma once

#include "vulkan/common.h"
#include "vulkan/core/ShaderModule.h"
#include "vulkan/core/RenderPass.h"
#include "vulkan/rendering/RenderTarget.h"

namespace vk
{

template<class T>
inline void hash_combine(size_t& seed, const T& b)
{
    std::hash<T> hasher;
    size_t hash = hasher(b);

    // Taken from glm::detail::hash_combine
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

} // vk

namespace std
{

template<typename T>
struct hash<vector<T>>
{
    size_t operator()(const vector<T>& tVec) const
    {
        size_t result = 0;
        for( auto t : tVec )
        {
            vk::hash_combine(result, t);
        }

        return result;
    }
};

template<>
struct hash<VkExtent2D>
{
    size_t operator()(const VkExtent2D& extent) const
    {
        size_t result = 0;
        vk::hash_combine(result, extent.width);
        vk::hash_combine(result, extent.height);
        return result;
    }
};

template<>
struct hash<VkExtent3D>
{
    size_t operator()(const VkExtent3D& extent) const
    {
        size_t result = 0;
        vk::hash_combine(result, VkExtent2D{ extent.width, extent.height });
        vk::hash_combine(result, extent.depth);
        return result;
    }
};

template<>
struct hash<VkDescriptorType>
{
    size_t operator()(const VkDescriptorType& type) const
    {
        std::hash<std::underlying_type<VkDescriptorType>::type> hasher;
        return hasher(type);
    }
};

template<>
struct hash<vk::Device>
{
    size_t operator()(const vk::Device& device) const
    {
        return 0;
    }
};

template<>
struct hash<vk::ShaderModule>
{
    size_t operator()(const vk::ShaderModule& shaderModule) const
    {
        size_t result = 0;
        vk::hash_combine(result, shaderModule.get_uid());
        return result;
    }
};

template<>
struct hash<VkShaderStageFlagBits>
{
    size_t operator()(const VkShaderStageFlagBits& bits) const
    {
        std::hash<uint32_t> hasher;
        return hasher(static_cast<uint32_t>(bits));
    }
};

template<>
struct hash<vector<uint8_t>>
{
    size_t operator()(const vector<uint8_t>& vec) const
    {
        std::hash<string> strhash;
        return strhash(std::string(vec.begin(), vec.end()));
    }
};

template<>
struct hash<vk::ShaderResource>
{
    size_t operator()(const vk::ShaderResource& resource) const
    {
        size_t result = 0;

        if( resource.type == vk::ShaderResourceType::Input ||
            resource.type == vk::ShaderResourceType::Output ||
            resource.type == vk::ShaderResourceType::PushConstant ||
            resource.type == vk::ShaderResourceType::SpecializationConstant )
        {
            return result;
        }

        vk::hash_combine(result, resource.set);
        vk::hash_combine(result, resource.binding);
        vk::hash_combine(result, static_cast<std::underlying_type<vk::ShaderResourceMode>::type>(resource.type));
        vk::hash_combine(result, resource.mode);

        return result;
    }
};

template<>
struct hash<vk::RenderPass>
{
    size_t operator()(const vk::RenderPass& pass) const
    {
        size_t result = 0;
        vk::hash_combine(result, pass.get_handle());
        return result;
    }
};

template<>
struct hash<vk::RenderTarget>
{
    size_t operator()(const vk::RenderTarget& target) const
    {
        size_t result = 0;

        for( auto& view : target.get_image_views() )
        {
            vk::hash_combine(result, view.get_handle());
            vk::hash_combine(result, view.get_image().get_handle());
        }

        return result;
    }
};

} // std

namespace vk
{

template<typename T, typename... A>
void hash_params(size_t& seed, const T& param)
{
    hash_combine(seed, param);
}

template<typename T, typename... A>
void hash_params(size_t& seed, const T& firstArg, const A&... args)
{
    hash_combine(seed, firstArg);
    hash_params(seed, args...);
}

} // vk
