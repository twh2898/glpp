#include "glpp/extra/Transform.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace glpp::extra {
    Transform::Transform()
        : Transform(glm::vec3(0), glm::quat(glm::vec3(0)), glm::vec3(1)) {}

    Transform::Transform(const glm::vec3 & position,
                         const glm::quat & rotation,
                         const glm::vec3 & scale)
        : m_position(position),
          m_rotation(rotation),
          m_scale(scale),
          changed(true) {}

    // Set changed to true so matrix will be re-calculated on first call to
    // toMatrix(). Input matrix could have skew or perspective that are not
    // captured by this class.
    Transform::Transform(const glm::mat4 & matrix)
        : matrix(matrix), changed(true) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, m_scale, m_rotation, m_position, skew, perspective);
        // m_rotation = glm::conjugate(m_rotation);
    }

    Transform::~Transform() {}

    void Transform::move(const glm::vec3 & delta) {
        m_position += delta;
        changed = true;
    }

    void Transform::rotateEuler(const glm::vec3 & delta) {
        rotate(glm::quat(delta));
        changed = true;
    }

    void Transform::rotate(const glm::quat & delta) {
        // Quaternion rotation is second * first which is why delta is first
        m_rotation = delta * m_rotation;
        changed = true;
    }

    void Transform::scale(const glm::vec3 & scale) {
        m_scale *= scale;
        changed = true;
    }

    const glm::vec3 & Transform::getPosition() const {
        return m_position;
    }

    void Transform::setPosition(const glm::vec3 & position) {
        m_position = position;
        changed = true;
    }

    glm::vec3 Transform::getRotationEuler() const {
        return glm::eulerAngles(m_rotation);
    }

    const glm::quat & Transform::getRotation() const {
        return m_rotation;
    }

    void Transform::setRotation(const glm::quat & rotation) {
        m_rotation = rotation;
        changed = true;
    }

    const glm::vec3 & Transform::getScale() const {
        return m_scale;
    }

    void Transform::setScale(const glm::vec3 & scale) {
        m_scale = scale;
        changed = true;
    }

    glm::mat4 Transform::toMatrix() const {
        if (changed) {
            auto translate = glm::translate(glm::mat4(1), m_position);
            auto rotate = glm::toMat4(m_rotation);
            auto scale = glm::scale(glm::mat4(1), m_scale);
            matrix = translate * rotate * scale;
            changed = false;
        }

        return matrix;
    }
}