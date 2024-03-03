#include "Camera.h"

Camera::Camera(glm::mat4 projectionMatrix) :
    m_position(0.f, 0.f, 0.f),
    m_rotation(glm::quat(glm::vec3(0.f))),
    m_matrices({ projectionMatrix, glm::mat4(1.f) }),
    m_dirty(true)
{
    m_matrices.projection[1][1] *= -1;
}

void Camera::rotate(const glm::vec3& eulerRotation)
{
    rotate(glm::angleAxis(eulerRotation.z, glm::vec3(0.f, 0.f, 1.f)));
    rotate(glm::angleAxis(eulerRotation.y, glm::vec3(0.f, 1.f, 0.f)));
    rotate(glm::angleAxis(eulerRotation.x, glm::vec3(1.f, 0.f, 0.f)));
}

void Camera::rotate(const glm::quat& rotation)
{
    m_rotation *= rotation;
    m_dirty = true;
}

void Camera::translate(const glm::vec3& translation)
{
    if( translation.length() == 0.f )
    {
        return;
    }

    m_position += translation;
    m_dirty = true;
}

glm::vec3 Camera::get_position() const
{
    return m_position;
}

glm::vec3 Camera::get_euler_rotation() const
{
    return glm::eulerAngles(m_rotation);
}

glm::quat Camera::get_rotation() const
{
    return m_rotation;
}

glm::mat4 Camera::get_projection_matrix()
{
    return m_matrices.projection;
}

glm::mat4 Camera::get_view_matrix()
{
    if( m_dirty )
    {
        return glm::toMat4(m_rotation) * glm::translate(glm::mat4(1.f), -m_position);
    }

    return m_matrices.view;
}

const Camera::MatrixData& Camera::get_matrix_data()
{
    if( m_dirty )
    {
        m_matrices.view = get_view_matrix();
        m_dirty = false;
    }
    return m_matrices;
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearZ, float farZ) :
    Camera(glm::perspective(fov, aspect, nearZ, farZ)),
    m_fov(fov),
    m_aspect(aspect),
    m_nearZ(nearZ),
    m_farZ(farZ)
{ }

void PerspectiveCamera::set_fov(float fov)
{
    if( fov == m_fov )
    {
        return;
    }

    m_fov = fov;
    m_matrices.projection = glm::perspective(fov, m_aspect, m_nearZ, m_farZ);
    m_matrices.projection[1][1] *= -1;
    m_dirty = true;
}

void PerspectiveCamera::set_aspect(float aspect)
{
    if( m_aspect == aspect )
    {
        return;
    }

    m_aspect = aspect;
    m_matrices.projection = glm::perspective(m_fov, m_aspect, m_nearZ, m_farZ);
    m_matrices.projection[1][1] *= -1;
    m_dirty = true;
}

float PerspectiveCamera::get_fov() const
{
    return m_fov;
}