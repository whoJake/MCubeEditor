#pragma once

template<class T = float>
struct AABoundingBox
{
    static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
        "T only supports types <float, double>");
    
    glm::vec<3, T> min;
    glm::vec<3, T> max;

    inline constexpr glm::vec<3, T> centre() const
    {
        return (min + max) / static_cast<T>(2.0);
    }

    inline constexpr glm::vec<3, T> extent() const
    {
        return (max - min) / static_cast<T>(2.0);
    }

    inline constexpr bool contains(const glm::vec<3, T>& point) const
    {
        return point.x >= min.x && point.x <= max.x
            && point.y >= min.y && point.y <= max.y
            && point.z >= min.z && point.z <= max.z;
    }

    inline constexpr bool intersects(const AABoundingBox<T>& other) const
    {
        return other.min.x <= max.x && other.max.x >= min.x
            && other.min.y <= max.y && other.max.y >= min.y
            && other.min.z <= max.z && other.max.z >= min.z;
    }

    inline constexpr auto operator<=>(const AABoundingBox<T>&) const = default;
};

template<class T = float>
struct BoundingSphere
{
    static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
        "T only supports types <float, double>");

    glm::vec<3, T> centre;
    T radius;

    inline constexpr bool contains(const glm::vec<3, T>& point) const
    {
        return std::abs(point - centre) <= radius;
    }

    inline constexpr bool intersects(const BoundingSphere<T>& other) const
    {
        return std::abs(other.centre - centre) <= radius + other.radius;
    }

    inline constexpr auto operator<=>(const BoundingSphere<T>&) const = default;
};

class Transform
{
public:
    Transform(const glm::vec3& position = { 0.f, 0.f, 0.f },
              const glm::vec3& scale    = { 1.f, 1.f, 1.f },
              const glm::quat& rotation = glm::quat({ 0.f, 0.f, 0.f })) :
        m_position(position),
        m_scale(scale),
        m_rotation(rotation)
    { }
    ~Transform() = default;
    Transform(Transform&&) = default;
    Transform(const Transform&) = default;
    Transform& operator=(Transform&&) = default;
    Transform& operator=(const Transform&) = default;

    inline const glm::vec3& position() const
    {
        return m_position;
    }

    inline glm::vec3& position()
    {
        return m_position;
    }

    inline const glm::vec3& scale() const
    {
        return m_scale;
    }

    inline glm::vec3& scale()
    {
        return m_scale;
    }

    inline const glm::quat& rotation() const
    {
        return m_rotation;
    }

    inline glm::quat& rotation()
    {
        return m_rotation;
    }

    inline glm::mat4 as_matrix() const
    {
        return glm::translate((glm::scale(glm::mat4(1.f), m_scale) * glm::toMat4(m_rotation)), m_position);
    }

    inline void rotate(const glm::quat& rotation)
    {
        m_rotation *= rotation;
    }

    inline void rotate(const glm::vec3& eulerRotation)
    {
        m_rotation *= glm::quat(eulerRotation);
    }

    inline void translate(const glm::vec3& translation)
    {
        m_position += translation;
    }

    inline constexpr auto operator<=>(const Transform&) const = default;
private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::quat m_rotation;
};