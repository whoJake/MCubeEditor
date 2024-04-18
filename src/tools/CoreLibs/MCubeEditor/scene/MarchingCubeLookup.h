#pragma once

class MarchingCubeLookup
{
public:
    MarchingCubeLookup();

    const std::array<int8_t, 16>& get_edges_for_state(uint8_t state) const;

    constexpr glm::ivec3 get_corner_offset(uint32_t index) const
    {
        constexpr std::array<glm::ivec3, 8> offsets(
            {
                { 0, 0, 0 },
                { 1, 0, 0 },
                { 1, 0, 1 },
                { 0, 0, 1 },
                { 0, 1, 0 },
                { 1, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 1 }
            });
        return offsets[index];
    }

    constexpr std::pair<uint8_t, uint8_t> get_corners_by_edge(uint8_t edgeIndex) const
    {
        constexpr std::array<uint8_t, 12> edgeCornersA{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3 };
        constexpr std::array<uint8_t, 12> edgeCornersB{ 1, 2, 3, 0, 5, 6, 7, 4, 4, 5, 6, 7 };
        return std::make_pair(edgeCornersA[edgeIndex], edgeCornersB[edgeIndex]);
    }
private:
    std::array<std::array<int8_t, 16>, 256> m_triangulation;
public:
    static MarchingCubeLookup *const instance()
    {
        if( !s_instance )
        {
            s_instance = new MarchingCubeLookup();
        }

        return s_instance;
    }
private:
    inline static MarchingCubeLookup* s_instance = nullptr;
};