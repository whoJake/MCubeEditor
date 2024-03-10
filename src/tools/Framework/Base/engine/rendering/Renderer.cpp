#include "Renderer.h"

#include "common/fileio.h"
#include "vulkan/core/Pipeline.h"
#include "Blueprint.h"

Renderer::Renderer(vk::RenderContext& context) :
    m_context(context)
{
    create_debug_material();
}

Renderer::~Renderer()
{
    m_context.get_device().wait_idle();

    delete m_debugMaterial.pipeline;
    delete m_debugMaterial.renderPass;
    delete m_debugMaterial.pipelineLayout;
}

void Renderer::render_scene(Scene& scene, Camera& camera)
{
    vk::CommandBuffer& mainCmdBuffer = m_context.begin(vk::CommandBuffer::ResetMode::AlwaysAllocate);
    vk::RenderTarget& activeTarget = m_context.get_active_frame().get_render_target();

    // debug
    vk::Framebuffer& activeFramebuffer = m_context.get_device().get_resource_cache().request_framebuffer(activeTarget, *m_debugMaterial.renderPass);

    mainCmdBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr, &activeFramebuffer, 0);

    VkClearValue color{ };
    color.color = { .2f, .2f, .2f, 1.f };
    VkClearValue depth{ };
    depth.depthStencil = { 1.f, 0 };

    std::vector<VkClearValue> clearColours;
    clearColours.push_back(color);
    clearColours.push_back(depth);

    mainCmdBuffer.begin_render_pass(&activeTarget, *m_debugMaterial.renderPass, activeFramebuffer, clearColours);
    mainCmdBuffer.bind_pipeline(*m_debugMaterial.pipeline);

    
    VkViewport viewport{ };
    viewport.y = static_cast<float>(activeTarget.get_extent().height);
    viewport.width = static_cast<float>(activeTarget.get_extent().width);
    viewport.height = -static_cast<float>(activeTarget.get_extent().height);
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;
    mainCmdBuffer.set_viewport(viewport);

    VkRect2D scissor{ };
    scissor.offset = { 0, 0 };
    scissor.extent = activeTarget.get_extent();
    mainCmdBuffer.set_scissor(scissor);

    // Set camera
    mainCmdBuffer.push_constants(
        *m_debugMaterial.pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(Camera::MatrixData),
        &camera.get_matrix_data());


    for( Entity* entity : scene.get_scene_entities() )
    {
        glm::mat4 model = entity->transform().as_matrix();

        // Set model
        mainCmdBuffer.push_constants(
            *m_debugMaterial.pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT,
            sizeof(Camera::MatrixData),
            sizeof(glm::mat4),
            &model);

        Blueprint& blueprint = entity->blueprint();
        mainCmdBuffer.bind_vertex_buffers(blueprint.mesh_renderer().get_vertex_buffer(m_context), 0);
        mainCmdBuffer.bind_index_buffer(blueprint.mesh_renderer().get_index_buffer(m_context), VK_INDEX_TYPE_UINT16);

        mainCmdBuffer.draw_indexed(vk::to_u32(blueprint.mesh().get_index_count()));

        blueprint.mesh().set_dirty(false);
    }

    mainCmdBuffer.end_render_pass();
    mainCmdBuffer.end();

    m_context.submit_and_end(mainCmdBuffer);
}

void Renderer::create_debug_material()
{
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

    m_debugMaterial.renderPass = new vk::RenderPass(m_context.get_device(), attachments, infos, subpassInfos);

    std::string vertPath = "shaders/basic.vert";
    std::vector<char> vertSource = FileIO::try_read_file(vertPath).value();
    std::vector<uint8_t> vertSrc(vertSource.size());
    memcpy(vertSrc.data(), vertSource.data(), vertSrc.size());

    std::string fragPath = "shaders/basic.frag";
    std::vector<char> fragSource = FileIO::try_read_file(fragPath).value();
    std::vector<uint8_t> fragSrc(fragSource.size());
    memcpy(fragSrc.data(), fragSource.data(), fragSrc.size());

    vk::ShaderModule& vertModule = m_context.get_device().get_resource_cache().request_shader_module(VK_SHADER_STAGE_VERTEX_BIT, vertSrc, "main");
    vk::ShaderModule& fragModule = m_context.get_device().get_resource_cache().request_shader_module(VK_SHADER_STAGE_FRAGMENT_BIT, fragSrc, "main");

    std::vector<vk::ShaderModule*> modules({ &vertModule, &fragModule });

    m_debugMaterial.pipelineLayout = new vk::PipelineLayout(m_context.get_device(), modules);
    m_debugMaterial.pipelineState.set_pipeline_layout(*m_debugMaterial.pipelineLayout);
    m_debugMaterial.pipelineState.set_render_pass(*m_debugMaterial.renderPass);

    VkVertexInputBindingDescription bindingDescription{ };
    bindingDescription.binding = 0;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindingDescription.stride = sizeof(glm::vec3) + sizeof(glm::vec3);

    VkVertexInputAttributeDescription attributeDescription{ };
    attributeDescription.binding = 0;
    attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription.location = 0;
    attributeDescription.offset = 0;

    VkVertexInputAttributeDescription attributeDescription2{ };
    attributeDescription2.binding = 0;
    attributeDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription2.location = 1;
    attributeDescription2.offset = sizeof(glm::vec3);

    vk::VertexInputStageState inputStage{ };
    inputStage.bindings.push_back(bindingDescription);
    inputStage.attributes.push_back(attributeDescription);
    inputStage.attributes.push_back(attributeDescription2);

    m_debugMaterial.pipelineState.set_vertex_input_state(inputStage);

    vk::ColorBlendState colorstate;
    colorstate.attachments.push_back(vk::ColorBlendAttachmentState());
    m_debugMaterial.pipelineState.set_color_blend_state(colorstate);

    m_debugMaterial.pipeline = new vk::GraphicsPipeline(m_context.get_device(), VK_NULL_HANDLE, m_debugMaterial.pipelineState);
}