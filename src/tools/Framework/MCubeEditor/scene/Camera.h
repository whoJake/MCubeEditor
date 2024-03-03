#pragma once

#include "common/stdincludes.h"
#include "glm.hpp"
#include "gtx/quaternion.hpp"

#include "vulkan/core/Buffer.h"

class Camera
{
public:
    struct MatrixData
    {
        glm::mat4 projection;
        glm::mat4 view;
    };

    virtual ~Camera() { };

    void rotate(const glm::vec3& eulerRotation);
    void rotate(const glm::quat& rotation);

    void translate(const glm::vec3& translation);

    glm::vec3 get_position() const;

    glm::vec3 get_euler_rotation() const;

    glm::quat get_rotation() const;

    glm::mat4 get_projection_matrix();

    glm::mat4 get_view_matrix();

    const MatrixData& get_matrix_data();
protected:
    Camera(glm::mat4 projectionMatrix);
protected:
    glm::vec3 m_position;
    glm::quat m_rotation;

    MatrixData m_matrices;

    bool m_dirty;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspect, float nearZ, float farZ);
    ~PerspectiveCamera() { };

    void set_fov(float fov);

    void set_aspect(float aspect);

    float get_fov() const;
private:
    float m_fov;
    float m_aspect;
    float m_nearZ;
    float m_farZ;
};