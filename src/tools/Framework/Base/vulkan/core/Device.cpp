#include "Device.h"

#include <algorithm>
#include "Instance.h"

namespace vk
{

Device::Device(jclog::Log&              log,
               PhysicalDevice&          gpu,
               VkSurfaceKHR             surface,
               std::vector<std::string> requestedExtensions) :
    m_log(log),
    m_gpu(gpu),
    m_surface(surface),
    m_resourceCache(*this)
{
    JCLOG_NONE(log, "{} GPU selected.", std::string(gpu.get_properties().deviceName).c_str());

    //--Prepare the device queues--
    uint32_t queueFamilyCount = to_u32(m_gpu.get_queue_family_properties().size());
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(queueFamilyCount, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO });
    std::vector<std::vector<float>> priorities(queueFamilyCount);

    for( uint32_t familyIndex = 0; familyIndex < queueFamilyCount; familyIndex++ )
    {
        const VkQueueFamilyProperties& familyProperties = m_gpu.get_queue_family_properties()[familyIndex];
        priorities[familyIndex] = std::vector<float>(familyProperties.queueCount, 0.5f);

        VkDeviceQueueCreateInfo& createInfo = queueCreateInfos[familyIndex];
        createInfo.queueFamilyIndex = familyIndex;
        createInfo.queueCount = familyProperties.queueCount;
        createInfo.pQueuePriorities = priorities[familyIndex].data();
    }

    //--Check extensions--
    uint32_t deviceExtensionCount{ 0 };
    vkEnumerateDeviceExtensionProperties(m_gpu.get_handle(), nullptr, &deviceExtensionCount, nullptr);

    m_deviceExtensions.resize(deviceExtensionCount);
    vkEnumerateDeviceExtensionProperties(m_gpu.get_handle(), nullptr, &deviceExtensionCount, m_deviceExtensions.data());

    //--Display available extensions
    if( deviceExtensionCount != 0 )
        JCLOG_INFO(log, "Device supports the following extensions:");
    for( VkExtensionProperties& property : m_deviceExtensions )
    {
        JCLOG_INFO(log, "\t{}", std::string(property.extensionName).c_str());
    }


    std::vector<const char*> unsupportedExtensions{ };
    for( std::string& extension : requestedExtensions )
    {
        if( is_extension_supported(extension) )
        {
            m_enabledExtensions.push_back(extension.c_str());
        }
        else
        {
            unsupportedExtensions.push_back(extension.c_str());
        }
    }

    if( !unsupportedExtensions.empty() )
    {
        for( const char* extension : unsupportedExtensions )
        {
            JCLOG_ERROR(log, "{} is required but not supported", extension);
        }
        throw VulkanException("Extensions required but not supported. See above for details.");
    }

    //--Create device--
    VkDeviceCreateInfo deviceCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    deviceCreateInfo.queueCreateInfoCount = queueFamilyCount;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount = to_u32(m_enabledExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = m_enabledExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &m_gpu.get_requested_features();

    VkResult deviceResult = vkCreateDevice(m_gpu.get_handle(), &deviceCreateInfo, nullptr, &m_handle);
    VK_CHECK(deviceResult, "Failed to create Logical Device.");

    VmaAllocatorCreateInfo allocCreateInfo{ };
    allocCreateInfo.vulkanApiVersion = gpu.get_instance().get_api_version();
    allocCreateInfo.physicalDevice = gpu.get_handle();
    allocCreateInfo.device = get_handle();
    allocCreateInfo.instance = gpu.get_instance().get_handle();

    VkResult allocResult = vmaCreateAllocator(&allocCreateInfo, &m_allocator);
    VK_CHECK(allocResult, "Failed to create VMA allocator.");

    //--Assign queues--
    for( uint32_t familyIndex = 0; familyIndex < queueFamilyCount; familyIndex++ )
    {
        const VkQueueFamilyProperties& familyProperties = m_gpu.get_queue_family_properties()[familyIndex];
        m_queues.push_back({ });

        for( uint32_t index = 0; index < familyProperties.queueCount; index++ )
        {
            m_queues[familyIndex].emplace_back(*this, familyIndex, familyProperties, m_gpu.is_present_supported(m_surface, familyIndex), index);
        }
    }
}

Device::~Device()
{
    m_resourceCache.clear();
    vmaDestroyAllocator(m_allocator);
    vkDestroyDevice(m_handle, nullptr);
}

VkDevice Device::get_handle() const
{
    return m_handle;
}

const jclog::Log& Device::get_log() const
{
    return m_log;
}

const PhysicalDevice& Device::get_gpu() const
{
    return m_gpu;
}

const Queue& Device::get_queue(uint32_t familyIndex, uint32_t index) const
{
    return m_queues[familyIndex][index];
}

const Queue& Device::get_queue_by_flags(VkQueueFlags flags, uint32_t index) const
{
    for( size_t familyIndex = 0; familyIndex < m_queues.size(); familyIndex++ )
    {
        const Queue& first = m_queues[familyIndex][0];
        if( (first.get_properties().queueFlags & flags) == flags )
            return m_queues[familyIndex][index];
    }
    throw VulkanException("Queue index {} was not found in VkDevice.", index);
}

const Queue& Device::get_queue_by_present(uint32_t index) const
{
    for( size_t familyIndex = 0; familyIndex < m_queues.size(); familyIndex++ )
    {
        const Queue& first = m_queues[familyIndex][0];
        if( first.has_present_support() )
            return m_queues[familyIndex][index];
    }
    throw VulkanException("No present queue has index {}.", index);
}

bool Device::is_extension_supported(std::string extension) const
{
    for( const VkExtensionProperties& property : m_deviceExtensions )
    {
        if( strcmp(property.extensionName, extension.c_str()) == 0 )
            return true;
    }
    return false;
}

bool Device::is_extension_enabled(std::string extension) const
{
    for( const char* property : m_enabledExtensions )
    {
        if( strcmp(property, extension.c_str()) == 0 )
            return true;
    }
    return false;
}

VkResult Device::wait_idle() const
{
    return vkDeviceWaitIdle(m_handle);
}

VmaAllocator Device::get_allocator() const
{
    return m_allocator;
}

ResourceCache& Device::get_resource_cache()
{
    return m_resourceCache;
}

} // vk