#include "WindowedApplication.h"

#include "Windows.h"
#include "platform/Window.h"
#include "platform/implementation/WindowGlfw.h"
#include <cstdlib>

#include <sstream>

#include "vulkan/core/Instance.h"
#include "vulkan/core/PhysicalDevice.h"
#include "vulkan/core/Device.h"
#include "vulkan/core/ShaderModule.h"
#include "vulkan/core/DescriptorSetLayout.h"
#include "vulkan/core/DescriptorPool.h"
#include "vulkan/core/DescriptorSet.h"

#include "common/fileio.h"

WindowedApplication::WindowedApplication()
{ }

WindowedApplication::~WindowedApplication()
{ }

ExitFlags WindowedApplication::app_main()
{
    Window::Properties winProperties;
    winProperties.mode = Window::Mode::Windowed;
    winProperties.extent = { 500, 500 };
    winProperties.resizable = true;
    winProperties.vsync = Window::VSync::Default;
    winProperties.eventfn = BIND_EVENT_FN(WindowedApplication::on_event);

    Window* glfw = new WindowGlfw(get_log(), winProperties);
    glfw->set_title("This is the title.");

    std::vector<const char*> vkInstanceExtensions = glfw->get_required_surface_extensions();
    vkInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    vk::Instance vkInstance(get_log(),
        "MCubeEditor",
        "vk",
        VK_API_VERSION_1_3,
        vkInstanceExtensions,
        { "VK_LAYER_KHRONOS_validation" });

    VkSurfaceKHR vkSurface = glfw->create_surface(vkInstance);

    vk::Device vkDevice(get_log(),
        vkInstance.get_first_gpu(),
        vkSurface);

    JCLOG_INFO(get_log(), "{} limits.", vkDevice.get_gpu().get_properties().limits.timestampPeriod);

    {
        std::string shaderPath = "shaders/Atomics.frag";
        std::vector<char> sourceBytes = FileIO::try_read_file(shaderPath).value();
        std::vector<uint8_t> source(sourceBytes.size());
        memcpy(source.data(), sourceBytes.data(), source.size());

        vk::ShaderModule& shaderModule = vkDevice.get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, source, "main");
        std::vector<vk::ShaderModule*> shaderModuleVec;
        shaderModuleVec.push_back(&shaderModule);

        vk::DescriptorSetLayout setLayout(vkDevice, 0, shaderModuleVec, shaderModule.get_resources());
        vk::DescriptorPool descriptorPool(vkDevice, setLayout);

        // Test expanding pool
        for( int i = 0; i < 35; i++ )
        {
            vk::DescriptorSet set(vkDevice, descriptorPool);
        }
    }

    while( !glfw->get_should_close() )
    {
        glfw->process_events();
    }

    vkDestroySurfaceKHR(vkInstance.get_handle(), vkSurface, nullptr);
    delete glfw;

    return ExitFlagBits::Success;
}

void WindowedApplication::on_event(Event& e)
{
    get_log().event(__FUNCTION__, "{}", e.to_str());
}