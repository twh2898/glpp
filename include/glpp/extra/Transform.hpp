#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace glpp::extra {
    class Transform {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        mutable glm::mat4 matrix;
        mutable bool changed;

    public:
        Transform();

        Transform(const glm::vec3 & position,
                  const glm::quat & rotation,
                  const glm::vec3 & scale);

        Transform(const glm::mat4 & matrix);

        Transform(const Transform & other) = default;

        Transform(Transform && other) = default;

        Transform & operator=(const Transform & other) = default;

        Transform & operator=(Transform && other) = default;

        ~Transform();

        void move(const glm::vec3 & delta);

        void rotateEuler(const glm::vec3 & delta);

        void rotate(const glm::quat & delta);

        void scale(const glm::vec3 & scale);

        const glm::vec3 & getPosition() const;

        void setPosition(const glm::vec3 & position);

        glm::vec3 getRotationEuler() const;

        const glm::quat & getRotation() const;

        void setRotation(const glm::quat & rotation);

        const glm::vec3 & getScale() const;

        void setScale(const glm::vec3 & scale);

        glm::mat4 toMatrix() const;
    };
}
