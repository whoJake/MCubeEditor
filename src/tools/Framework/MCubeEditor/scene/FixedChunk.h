#pragma once

#include "common/stdincludes.h"
#include "glm.hpp"
#include "vulkan/core/Buffer.h"

namespace vk
{
class Device;
}

class FixedChunk
{
public:
    struct Data
    {
        std::vector<glm::vec3> vertices{ };
        std::vector<uint32_t> indices{ };
    };

    FixedChunk(vk::Device& device, const glm::vec3 origin, const glm::uvec3 dimensions = { 16, 16, 16 });
    ~FixedChunk();

    const Data& get_data() const;

    size_t get_triangle_count() const;

    const glm::uvec3& get_dimensions() const;

    vk::Buffer& get_vertex_buffer();

    vk::Buffer& get_index_buffer();
private:
    void update_buffer_mapping();
private:
    glm::uvec3 m_dimensions;
    glm::vec3 m_origin;
    Data m_data{ };

    bool m_dirty;

    std::unique_ptr<vk::Buffer> m_vertexBuffer;
    std::unique_ptr<vk::Buffer> m_indexBuffer;
};