#pragma once

#include "pch/assert.h"
#include "helpers/easing_functions.h"
#include "LookupData.h"
#include "threading/JobDispatcher.h"

#include <chrono>

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
    explicit Volume(glm::uvec3 dimensions, T minValue, T maxValue, T threshold) :
        m_data(dimensions.x * dimensions.y * dimensions.z, minValue),
        m_minValue(minValue),
        m_maxValue(maxValue),
        m_threshold(threshold),
        m_dimensions(dimensions)
    {
        TRAP_LT(threshold, minValue, "Threshold is lower than minimum value.");
        TRAP_GT(threshold, maxValue, "Threshold is greater than maximum value.");
    }

    ~Volume()
    { }

    [[nodiscard]]
    inline MeshData calculate(CalculationFlags flags = MESH) const
    {
        auto curtime = std::chrono::high_resolution_clock::now();

        TRAP_EQ(flags, 0, "Invalid flags set to calculate.");
        TRAP_NEQ(flags & CalculationFlagBits::NORMALS, 0, "Normals is not yet supported.");

        MeshData retval;
        retval.vertices.reserve(
            static_cast<size_t>(m_dimensions.x)
            * static_cast<size_t>(m_dimensions.y)
            * static_cast<size_t>(m_dimensions.z)
            * 5u); // maximum 5 vertices per cube

        retval.indices.reserve(
            retval.vertices.capacity());

        if( flags & CalculationFlagBits::NORMALS )
        {
            retval.normals.reserve(
                retval.vertices.capacity());
        }

        if( flags & CalculationFlagBits::MULTITHREADED )
        {
            std::mutex addLock;

            auto perCubeJob = [=, &addLock, &retval](DispatchState state)
                {
                    glm::uvec3 origin = index_to_loc(state.jobIndex);
                    if( origin.x >= (m_dimensions.x - 1u) || origin.y >= (m_dimensions.y - 1u) || origin.z >= (m_dimensions.z - 1u) )
                    {
                        return;
                    }

                    TriangleData data = get_cube_triangles(
                        origin,
                        get_edges(origin),
                        static_cast<bool>(flags & CalculationFlagBits::NORMALS),
                        !static_cast<bool>(flags & CalculationFlagBits::NO_INTERPOLATION));

                    if( data.vertices.size() == 0 )
                    {
                        return;
                    }

                    // These seem pretty similar performance wise but bottom one looks way cooler ngl.
                #if 0
                    std::lock_guard<std::mutex> lock(addLock);
                    for( size_t i = 0; i < data.vertices.size(); i++ )
                    {
                        retval.vertices.push_back(data.vertices.at(i));
                        retval.indices.push_back(static_cast<uint16_t>(retval.indices.size()));
                        if( flags & CalculationFlagBits::NORMALS )
                        {
                            retval.normals.push_back(data.normals.at(i));
                        }
                    }
                #else
                    size_t beginIndex{ };
                    {
                        std::lock_guard<std::mutex> lock(addLock);
                        beginIndex = retval.vertices.size();
                        size_t size = beginIndex + data.vertices.size();

                        retval.vertices.resize(size);
                        retval.indices.resize(size);
                        if( flags & CalculationFlagBits::NORMALS )
                        {
                            retval.normals.resize(size);
                        }
                    }

                    for( size_t i = 0; i < data.vertices.size(); i++ )
                    {
                        retval.vertices.at(beginIndex + i) = data.vertices.at(i);
                        retval.indices.at(beginIndex + i) = static_cast<uint16_t>(beginIndex + i);
                        if( flags & CalculationFlagBits::NORMALS )
                        {
                            retval.normals.at(beginIndex + i) = data.normals.at(i);
                        }
                    }
                #endif
                };

            uint32_t jobCount = m_dimensions.x * m_dimensions.y * m_dimensions.z;
            JobDispatch::dispatch_and_wait(jobCount, 50, perCubeJob);
        }
        else
        {
            for( uint32_t x = 0; x < m_dimensions.x - 1; x++ )
            {
                for( uint32_t y = 0; y < m_dimensions.y - 1; y++ )
                {
                    for( uint32_t z = 0; z < m_dimensions.z - 1; z++ )
                    {
                        glm::uvec3 origin{ x, y, z };
                        TriangleData data = get_cube_triangles(
                            origin,
                            get_edges(origin),
                            static_cast<bool>(flags & CalculationFlagBits::NORMALS),
                            !static_cast<bool>(flags & CalculationFlagBits::NO_INTERPOLATION));

                        for( size_t i = 0; i < data.vertices.size(); i++ )
                        {
                            retval.indices.push_back(static_cast<uint32_t>(retval.indices.size()));
                            retval.vertices.push_back(data.vertices.at(i));
                            if( flags & CalculationFlagBits::NORMALS )
                            {
                                retval.normals.push_back(data.normals.at(i));
                            }
                        }
                    }
                }
            }
        }

        retval.vertices.shrink_to_fit();
        retval.indices.shrink_to_fit();
        if( flags & CalculationFlagBits::NORMALS )
        {
            retval.normals.shrink_to_fit();
        }

        return retval;
    }

    inline void add_local_sphere(glm::vec3 position, float radius, float multiplier, bool multithread = false, float(*easeFunc)(float) = easing_function_linear)
    {
        for( uint32_t x = 0; x < m_dimensions.x; x++ )
        {
            for( uint32_t y = 0; y < m_dimensions.y; y++ )
            {
                for( uint32_t z = 0; z < m_dimensions.z; z++ )
                {
                    glm::uvec3 location{ x, y, z };
                    glm::vec3 localPos = loc_to_local(location);
                    float distance = glm::distance(localPos, position);
                    if( distance > radius )
                    {
                        continue;
                    }
                    float percent = 1.f - (distance / radius);

                    T currentValue = at(location);
                    T rawDiff = static_cast<T>(m_minValue + ((m_maxValue - m_minValue) * easeFunc(percent)));
                    set(location, std::clamp(static_cast<T>(currentValue + (rawDiff * multiplier)), m_minValue, m_maxValue));
                }
            }
        }
    }

    inline glm::uvec3 get_dimensions() const
    {
        return m_dimensions;
    }
private:
    inline T& at(glm::uvec3 loc)
    {
        size_t index = loc_to_index(loc);
        return m_data.at(index);
    }

    inline const T& at(glm::uvec3 loc) const
    {
        size_t index = loc_to_index(loc);
        return m_data.at(index);
    }

    inline void set(glm::uvec3 loc, T value)
    {
        TRAP_LT(value, m_minValue, "Value is below the passed bounds.");
        TRAP_GT(value, m_maxValue, "Value is above the passed bounds.");

        at(loc) = value;
    }

    inline glm::uvec3 index_to_loc(size_t index) const
    {
        TRAP_GE(index, m_data.size(), "Index out of bounds.");

        return { index % m_dimensions.x,
            (index / m_dimensions.x) % m_dimensions.y,
            index / (m_dimensions.x * m_dimensions.y) };
    }

    inline size_t loc_to_index(glm::uvec3 loc) const
    {
        TRAP_GE(loc.x, m_dimensions.x, "Index out of bounds.");
        TRAP_GE(loc.y, m_dimensions.y, "Index out of bounds.");
        TRAP_GE(loc.z, m_dimensions.z, "Index out of bounds.");

        size_t retval{ 0 };
        retval += static_cast<size_t>(loc.x);
        retval += static_cast<size_t>(loc.y) * m_dimensions.x;
        retval += static_cast<size_t>(loc.z) * m_dimensions.x * m_dimensions.y;
        return retval;
    }

    inline glm::vec3 loc_to_local(glm::uvec3 loc) const
    {
        return {
            loc.x / static_cast<float>(m_dimensions.x - 1),
            loc.y / static_cast<float>(m_dimensions.y - 1),
            loc.z / static_cast<float>(m_dimensions.z - 1)
        };
    }

    inline const std::array<int8_t, 16>& get_edges(glm::uvec3 origin) const
    {
        LookupData* lookup = LookupData::instance();

        // get corner state
        uint8_t cornerState{ 0 };
        for( uint32_t corner = 0; corner < 8; corner++ )
        {
            glm::uvec3 offset = lookup->get_corner_offset(corner);
            glm::uvec3 cornerPoint{ origin.x + offset.x, origin.y + offset.y, origin.z + offset.z };

            if( at(cornerPoint) > m_threshold )
            {
                cornerState |= (1 << corner);
            }
        }

        return lookup->get_edges_for_state(cornerState);
    }

    inline TriangleData get_cube_triangles(glm::uvec3 origin, const std::array<int8_t, 16>& edges, bool calculateNormals, bool interpolate) const
    {
        LookupData* lookup = LookupData::instance();
        TriangleData retval{ };

        for( size_t edge = 0; edge < 16; edge += 3 )
        {
            if( edges[edge] == -1 )
            {
                break;
            }

            std::pair<uint8_t, uint8_t> eA = lookup->get_corners_by_edge(edges[edge]);
            std::pair<uint8_t, uint8_t> eB = lookup->get_corners_by_edge(edges[edge + 1]);
            std::pair<uint8_t, uint8_t> eC = lookup->get_corners_by_edge(edges[edge + 2]);

            // edge A
            glm::uvec3 eAvA = origin + lookup->get_corner_offset(eA.first);
            glm::uvec3 eAvB = origin + lookup->get_corner_offset(eA.second);

            glm::vec3 eAVertex{ };
            float edgeInterp = interpolate
                ? inverse_lerp(m_threshold, at(eAvA), at(eAvB))
                : 0.5f;

            eAVertex = lerp_points(loc_to_local(eAvA), loc_to_local(eAvB), edgeInterp);

            // edge B
            glm::uvec3 eBvA = origin + lookup->get_corner_offset(eB.first);
            glm::uvec3 eBvB = origin + lookup->get_corner_offset(eB.second);

            glm::vec3 eBVertex{ };
            edgeInterp = interpolate
                ? inverse_lerp(m_threshold, at(eBvA), at(eBvB))
                : 0.5f;

            eBVertex = lerp_points(loc_to_local(eBvA), loc_to_local(eBvB), edgeInterp);

            // edge C
            glm::uvec3 eCvA = origin + lookup->get_corner_offset(eC.first);
            glm::uvec3 eCvB = origin + lookup->get_corner_offset(eC.second);

            glm::vec3 eCVertex{ };
            edgeInterp = interpolate
                ? inverse_lerp(m_threshold, at(eCvA), at(eCvB))
                : 0.5f;

            eCVertex = lerp_points(loc_to_local(eCvA), loc_to_local(eCvB), edgeInterp);

            retval.vertices.push_back(eAVertex);
            retval.vertices.push_back(eBVertex);
            retval.vertices.push_back(eCVertex);
        }

        return retval;
    }

    inline float inverse_lerp(T threshold, T left, T right) const
    {
        return static_cast<float>(threshold - left) / (right - left);
    }

    inline glm::vec3 lerp_points(glm::vec3 a, glm::vec3 b, float value) const
    {
        return a + ((b - a) * value);
    }
private:
    mtl::fixed_vector<T> m_data;
    T m_minValue;
    T m_maxValue;
    T m_threshold;
    glm::uvec3 m_dimensions;
};

} // mcube