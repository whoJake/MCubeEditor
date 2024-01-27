#pragma once

#include "common/stdincludes.h"

#ifndef NOMINMAX
    #define NOMINMAX
#endif
#define VMA_STATIC_VULKAN_FUNCTIONS 1
#include "vma/vk_mem_alloc.h"

#define VK_CHECK(result, msg, ...)               \
    if(result!=VK_SUCCESS){                      \
        throw VulkanException(msg, __VA_ARGS__); \
    }

#define MAX_VK_FENCE_TIMEOUT = std::numeric_limits<size_t>::max();

namespace vk
{

struct LoadStoreInfo {
    VkAttachmentLoadOp loadOp;
    VkAttachmentStoreOp storeOp;
};

inline bool is_depth_only_format(VkFormat format) {
    return format == VK_FORMAT_D16_UNORM ||
        format == VK_FORMAT_D32_SFLOAT;
}

inline bool is_depth_stencil_format(VkFormat format) {
    return format == VK_FORMAT_D16_UNORM_S8_UINT ||
        format == VK_FORMAT_D24_UNORM_S8_UINT ||
        format == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

inline bool is_depth_format(VkFormat format) {
    return is_depth_only_format(format) || is_depth_stencil_format(format);
}

template <class T>
inline uint32_t to_u32(T val) {
    return static_cast<uint32_t>(val);
}

#define BYTE_TO_BINARY_STREAM(byte)  \
    ((byte) & 0x80 ? "1" : "0") << \
    ((byte) & 0x40 ? "1" : "0") << \
    ((byte) & 0x20 ? "1" : "0") << \
    ((byte) & 0x10 ? "1" : "0") << \
    ((byte) & 0x08 ? "1" : "0") << \
    ((byte) & 0x04 ? "1" : "0") << \
    ((byte) & 0x02 ? "1" : "0") << \
    ((byte) & 0x01 ? "1" : "0") 

template <typename T>
inline std::string to_bit_string(T val) {
    std::stringstream ss;
    ss << "0b";
    for( long long i = sizeof(T) - 1; i >= 0; i-- ) {
        T shiftval = static_cast<T>(i * 8);
        ss << BYTE_TO_BINARY_STREAM(val >> shiftval);
        ss << " ";
    }
    return ss.str();
}

/**
* @brief Reinterpret casts a VkPhysicalDeviceFeatures struct into an array of 55 VkBool's
*/
inline std::array<VkBool32, 55> to_boolarr(VkPhysicalDeviceFeatures features) {
    std::array<VkBool32, 55> result{ };
    VkBool32* arrptr = reinterpret_cast<VkBool32*>(&features);
    std::move(arrptr, arrptr + 55, result.begin());
    return result;
}

inline VkPhysicalDeviceFeatures operator_bitor(const VkPhysicalDeviceFeatures& first, const VkPhysicalDeviceFeatures& second) {
    std::array<VkBool32, 55> one = to_boolarr(first);
    std::array<VkBool32, 55> two = to_boolarr(second);

    std::array<VkBool32, 55> result{ };
    for( size_t i = 0; i < one.size(); i++ ) {
        result[i] = one[i] | two[i];
    }

    /// Cast back to VkPhysicalDeviceFeatures
    return *reinterpret_cast<VkPhysicalDeviceFeatures*>(&result[0]);
}

/**
* @brief Find the features that are either supported or unsupported between two feature sets
*
* @param "requested" The feature set that has been requested
* @param "available" The feature set that is available
* @param "supported" (Optional) Whether or not supported or unsupported features should be returned
*
* @returns Vector of the string names of supported or unsupported features
*/
static std::vector<std::string> get_device_features_overlap(const VkPhysicalDeviceFeatures& requested, const VkPhysicalDeviceFeatures& available, bool supported = true) {
    const std::string nameLookup[55]{
        "robustBufferAccess", "fullDrawIndexUint32", "imageCubeArray", "independentBlend", "geometryShader",
        "tessellationShader", "sampleRateShading", "dualSrcBlend", "logicOp", "multiDrawIndirect",
        "drawIndirectFirstInstance", "depthClamp", "depthBiasClamp", "fillModeNonSolid", "depthBounds",
        "wideLines", "largePoints", "alphaToOne", "multiViewport", "samplerAnisotropy", "textureCompressionETC2",
        "textureCompressionASTC_LDR", "textureCompressionBC", "occlusionQueryPrecise", "pipelineStatisticsQuery",
        "vertexPipelineStoresAndAtomics", "fragmentStoresAndAtomics", "shaderTessellationAndGeometryPointSize",
        "shaderImageGatherExtended", "shaderStorageImageExtendedFormats", "shaderStorageImageMultisample",
        "shaderStorageImageReadWithoutFormat", "shaderStorageImageWriteWithoutFormat", "shaderUniformBufferArrayDynamicIndexing",
        "shaderSampledImageArrayDynamicIndexing", "shaderStorageBufferArrayDynamicIndexing",
        "shaderStorageImageArrayDynamicIndexing", "shaderClipDistance", "shaderCullDistance", "shaderFloat64",
        "shaderInt64", "shaderInt16", "shaderResourceResidency", "shaderResourceMinLod", "sparseBinding",
        "sparseResidencyBuffer", "sparseResidencyImage2D", "sparseResidencyImage3D", "sparseResidency2Samples",
        "sparseResidency4Samples", "sparseResidency8Samples", "sparseResidency16Samples", "sparseResidencyAliased",
        "variableMultisampleRate", "inheritedQueries"
    };
    std::array<VkBool32, 55> req = to_boolarr(requested);
    std::array<VkBool32, 55> avail = to_boolarr(available);
    std::vector<std::string> result{ };

    for( size_t i = 0; i < req.size(); i++ ) {
        if( supported ) {
            if( req[i] && avail[i] )
                result.push_back(nameLookup[i]);
        }
        else {
            if( req[i] && !avail[i] )
                result.push_back(nameLookup[i]);
        }
    }

    return result;
}

} // vk