#pragma once

#include "common/stdincludes.h"
#include "Camera.h"
#include "FixedChunk.h"
#include "vulkan/rendering/RenderContext.h"
#include "vulkan/core/RenderPass.h"
#include "vulkan/core/PipelineLayout.h"
#include "vulkan/core/Pipeline.h"
#include "vulkan/core/PipelineState.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void create_material(vk::Device& device);

    void render(vk::RenderContext& renderContext, Camera& camera);
private:
    std::vector<FixedChunk*> m_chunks;

    struct
    {
        vk::RenderPass* renderPass{ nullptr };
        vk::PipelineLayout* pipelineLayout{ nullptr };
        vk::Pipeline* pipeline{ nullptr };
        vk::PipelineState pipelineState{ };
    }m_material{ };

    std::vector<VkClearValue> m_clearColors;
};