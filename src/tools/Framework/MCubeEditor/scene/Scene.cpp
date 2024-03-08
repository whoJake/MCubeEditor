#include "Scene.h"
#include "vulkan/core/Device.h"
#include "common/fileio.h"

#include "vulkan/core/ShaderModule.h"
#include "vulkan/core/CommandBuffer.h"

Scene::Scene()
{
    VkClearValue color{ };
    color.color = { .2f, .2f, .2f, 1.f };
    VkClearValue depth{ };
    depth.depthStencil = { 1.f, 0 };

    m_clearColors.push_back(color);
    m_clearColors.push_back(depth);

}

Scene::~Scene()
{
    m_material.pipeline->get_device().wait_idle();

    for( auto& chunk : m_chunks )
    {
        delete chunk;
    }

    delete m_material.pipeline;
    delete m_material.renderPass;
    delete m_material.pipelineLayout;
}

void Scene::render(vk::RenderContext& renderContext, Camera& camera)
{
    vk::CommandBuffer& mainCommandBuffer = renderContext.begin(vk::CommandBuffer::ResetMode::AlwaysAllocate);
    vk::RenderTarget& activeRenderTarget = renderContext.get_active_frame().get_render_target();
    vk::Framebuffer& activeFramebuffer = renderContext.get_device().get_resource_cache().request_framebuffer(activeRenderTarget, *m_material.renderPass);

    mainCommandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr, &activeFramebuffer, 0);

    mainCommandBuffer.begin_render_pass(&activeRenderTarget, *m_material.renderPass, activeFramebuffer, m_clearColors);
    mainCommandBuffer.bind_pipeline(*m_material.pipeline);

    // Set camera
    mainCommandBuffer.push_constants(*m_material.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Camera::MatrixData), &camera.get_matrix_data());

    VkViewport viewport{ };
    viewport.y = static_cast<float>(activeRenderTarget.get_extent().height);
    viewport.width = static_cast<float>(activeRenderTarget.get_extent().width);
    viewport.height = -static_cast<float>(activeRenderTarget.get_extent().height);
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;
    mainCommandBuffer.set_viewport(viewport);

    VkRect2D scissor{ };
    scissor.offset = { 0, 0 };
    scissor.extent = activeRenderTarget.get_extent();
    mainCommandBuffer.set_scissor(scissor);

    for( FixedChunk* chunk : m_chunks )
    {
        mainCommandBuffer.bind_vertex_buffers(chunk->get_vertex_buffer(), 0);
        mainCommandBuffer.bind_index_buffer(chunk->get_index_buffer());

        mainCommandBuffer.draw_indexed(vk::to_u32(chunk->get_data().indices.size()));
    }

    mainCommandBuffer.end_render_pass();
    mainCommandBuffer.end();

    renderContext.submit_and_end(mainCommandBuffer);
}

void Scene::create_material(vk::Device& device)
{
    glm::vec3 origin{ 0.f, 0.f, 0.f };
    FixedChunk* chunk = new FixedChunk(device, origin);
    m_chunks.push_back(chunk);

    std::vector<vk::Attachment> attachments({
        { VK_FORMAT_R8G8B8A8_UNORM,  VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT },
        { VK_FORMAT_D32_SFLOAT, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT }});

    std::vector<vk::LoadStoreInfo> infos({
        { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE },
        { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE }});

    std::vector<vk::SubpassInfo> subpassInfos({ { 
        { },
        { 0 },
        { },
        false,
        1,
        VK_RESOLVE_MODE_NONE,
        "Subpass" } });

    m_material.renderPass = new vk::RenderPass(device, attachments, infos, subpassInfos);

    std::string vertPath = "shaders/basic.vert";
    std::vector<char> vertSource = FileIO::try_read_file(vertPath).value();
    std::vector<uint8_t> vertSrc(vertSource.size());
    memcpy(vertSrc.data(), vertSource.data(), vertSrc.size());

    std::string fragPath = "shaders/basic.frag";
    std::vector<char> fragSource = FileIO::try_read_file(fragPath).value();
    std::vector<uint8_t> fragSrc(fragSource.size());
    memcpy(fragSrc.data(), fragSource.data(), fragSrc.size());

    vk::ShaderModule& vertModule = device.get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, vertSrc, "main");
    vk::ShaderModule& fragModule = device.get_resource_cache().request_shader_module(VK_SHADER_STAGE_FRAGMENT_BIT, fragSrc, "main");

    std::vector<vk::ShaderModule*> modules({ &vertModule, &fragModule });

    m_material.pipelineLayout = new vk::PipelineLayout(device, modules);
    m_material.pipelineState.set_pipeline_layout(*m_material.pipelineLayout);
    m_material.pipelineState.set_render_pass(*m_material.renderPass);

    VkVertexInputBindingDescription bindingDescription{ };
    bindingDescription.binding = 0;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindingDescription.stride = sizeof(glm::vec3);

    VkVertexInputAttributeDescription attributeDescription{ };
    attributeDescription.binding = 0;
    attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription.location = 0;
    attributeDescription.offset = 0;

    vk::VertexInputStageState inputStage{ };
    inputStage.bindings.push_back(bindingDescription);
    inputStage.attributes.push_back(attributeDescription);

    m_material.pipelineState.set_vertex_input_state(inputStage);

    vk::ColorBlendState colorstate;
    colorstate.attachments.push_back(vk::ColorBlendAttachmentState());
    m_material.pipelineState.set_color_blend_state(colorstate);

    m_material.pipeline = new vk::GraphicsPipeline(device, VK_NULL_HANDLE, m_material.pipelineState);

}