#pragma once

#include "helpers/easing_functions.h"

namespace mcube
{

struct MeshData
{
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;
};

enum CalculationFlagBits
{
    MESH = 1 << 0,
    NORMALS = 1 << 1,
    MULTITHREADED = 1 << 2,
    NO_INTERPOLATION = 1 << 3,

    ALL = MESH | NORMALS,
    ALL_MT = MESH | NORMALS | MULTITHREADED
};

using CalculationFlags = std::underlying_type<CalculationFlagBits>::type;

struct TriangleData
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
};

template<typename T>
class Volume
{
public:
    explicit Volume(glm::uvec3 dimensions, T minValue, T maxValue, T threshold);
    ~Volume();

    [[nodiscard]]
    MeshData calculate(CalculationFlags flags = MESH) const;

    void add_local_sphere(glm::vec3 position, float radius, float multiplier, bool multithread = false, float(*easeFunc)(float) = easing_function_linear);

    glm::uvec3 get_dimensions() const;
private:
    T& at(glm::uvec3 loc);
    const T& at(glm::uvec3 loc) const;

    void set(glm::uvec3 loc, T value);

    glm::uvec3 index_to_loc(size_t index) const;
    size_t loc_to_index(glm::uvec3 loc) const;

    glm::vec3 loc_to_local(glm::uvec3 loc) const;

    const std::array<int8_t, 16>& get_edges(glm::uvec3 origin) const;
    TriangleData get_cube_triangles(glm::uvec3 origin, const std::array<int8_t, 16>& edges, bool calculateNormals, bool interpolate) const;

    float inverse_lerp(T threshold, T left, T right) const;
    glm::vec3 lerp_points(glm::vec3 a, glm::vec3 b, float value) const;
private:
    mtl::fixed_vector<T> m_data;
    T m_minValue;
    T m_maxValue;
    T m_threshold;
    glm::uvec3 m_dimensions;
};

} // mcube