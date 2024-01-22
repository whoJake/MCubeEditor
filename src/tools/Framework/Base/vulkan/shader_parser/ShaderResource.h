#pragma once

#include "vulkan/common.h"

namespace vk
{

enum class ShaderResourceType {
    Input,
    InputAttachment,
    Output,
    Image,
    ImageSampler,
    ImageStorage,
    Sampler,
    BufferUniform,
    BufferStorage,
    PushConstant,
    SpecializationConstant,
    All
};

static VkDescriptorType get_descriptor_type(ShaderResourceType type, bool dynamic = false)
{
    switch( type )
    {
    case ShaderResourceType::InputAttachment:
        return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    case ShaderResourceType::Image:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case ShaderResourceType::ImageSampler:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case ShaderResourceType::ImageStorage:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case ShaderResourceType::Sampler:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case ShaderResourceType::BufferUniform:
        return dynamic
            ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
            : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case ShaderResourceType::BufferStorage:
        return dynamic
            ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
            : VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        break;
    default:
        throw VulkanException("Given resource type does not have a VkDescriptorType associated with it.");
    }
}

enum class ShaderResourceMode {
    Static,
    Dynamic,
    UpdateAfterBind
};

struct ShaderResourceQualifiers {
    enum : uint32_t {
        None = 0,
        NonReadable = 1,
        NonWriteable = 2
    };
};

struct ShaderResource {
    std::string name;
    ShaderResourceType type;
    VkShaderStageFlags stages;
    ShaderResourceMode mode;
    uint32_t qualifiers;

    uint32_t set;
    uint32_t binding;
    uint32_t location;
    uint32_t inputAttachmentIndex;

    uint32_t vecSize;
    uint32_t columns;
    uint32_t arraySize;
    uint32_t offset;
    uint32_t stride;
    uint32_t constantID;
};

} // vk