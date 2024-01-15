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