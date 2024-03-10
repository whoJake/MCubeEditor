#pragma once

#include "vulkan/rendering/RenderContext.h"
#include "engine/scene/Scene.h"
#include "Camera.h"


class Renderer
{
public:
    Renderer() = delete;
    Renderer(vk::RenderContext& context);

    ~Renderer();

    Renderer(Renderer&&) = delete;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void render_scene(Scene& scene, Camera& camera);
private:
    void create_debug_material();
private:
    vk::RenderContext& m_context;

    struct
    {
        vk::RenderPass* renderPass{ nullptr };
        vk::PipelineLayout* pipelineLayout{ nullptr };
        vk::Pipeline* pipeline{ nullptr };
        vk::PipelineState pipelineState{ };
    }m_debugMaterial{ };
};