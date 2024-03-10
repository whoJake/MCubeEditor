#include "Mesh.h"

template<class V, class T>
Mesh<V, T>::Mesh(const std::vector<V>& vertices,
                 const std::vector<T>& indices) :
    m_vertices(vertices),
    m_indices(indices),
    m_dirty(true)
{ }

template<class V, class T>
void Mesh<V, T>::set_vertex(size_t i, const V& vertex)
{
    m_vertices[i] = vertex;
    set_dirty();
}

template<class V, class T>
void Mesh<V, T>::set_index(size_t i, const T& index)
{
    m_indices[i] = index;
    set_dirty();
}

template<class V, class T>
void Mesh<V, T>::set_vertices(const std::vector<V>& vertices)
{
    m_vertices = vertices;
    set_dirty();
}

template<class V, class T>
void Mesh<V, T>::set_indices(const std::vector<T>& indices)
{
    m_indices = indices;
    set_dirty();
}

template<class V, class T>
const std::vector<V>& Mesh<V, T>::get_vertices() const
{
    return m_vertices;
}

template<class V, class T>
const std::vector<T>& Mesh<V, T>::get_indices() const
{
    return m_indices;
}

template<class V, class T>
size_t Mesh<V, T>::get_vertex_count() const
{
    return m_vertices.size();
}

template<class V, class T>
size_t Mesh<V, T>::get_stride() const
{
    return sizeof(V);
}

template<class V, class T>
size_t Mesh<V, T>::get_size() const
{
    return get_stride() * m_vertices.size();
}

template<class V, class T>
void Mesh<V, T>::set_dirty(bool value)
{
    m_dirty = value;
}

template<class V, class T>
bool Mesh<V, T>::get_dirty() const
{
    return m_dirty;
}