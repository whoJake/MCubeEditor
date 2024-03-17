#pragma once

#include "common/stdincludes.h"
#include "common/mathplus.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 colour;
};

// template<class V = Vertex, class T = uint16_t>
class Mesh
{
    /*
    static_assert(std::is_same_v<T, uint16_t>
               || std::is_same_v<T, uint32_t>,
        "Indices only supports types <uint16_t, uint32_t>");
        */
    typedef Vertex V;
    typedef uint16_t T;
public:
    Mesh(const std::vector<V>& vertices = { },
         const std::vector<T>& indices = { });

    ~Mesh() = default;
    Mesh(Mesh&&) = default;
    Mesh(const Mesh&) = default;

    void set_vertex(size_t i, const V& vertex);
    void set_index(size_t i, const T& index);
    void set_vertices(const std::vector<V>& vertices);
    void set_indices(const std::vector<T>& indices);

    const std::vector<V>& get_vertices() const;
    const std::vector<T>& get_indices() const;
    size_t get_vertex_count() const;
    size_t get_index_count() const;

    size_t get_vertex_stride() const;
    size_t get_vertices_size() const;
    size_t get_indices_size() const;

    void set_dirty(bool value = true);
    bool get_dirty() const;
private:
    std::vector<V> m_vertices;
    std::vector<T> m_indices;

    // Padding isnt great.
    bool m_dirty;
};
