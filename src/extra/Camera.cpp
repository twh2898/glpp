#include "glpp/extra/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace glpp::extra {
    constexpr float rotXMin = glm::radians(-89.f);
    constexpr float rotXMax = glm::radians(89.f);

    Camera::Camera() : Camera({0, 0}) {}

    Camera::Camera(const glm::uvec2 & screenSize,
                   ProjectionMode projection,
                   float fov,
                   const glm::vec3 & pos,
                   const glm::vec3 & rot,
                   float near,
                   float far)
        : screenSize(screenSize),
          projectionMode(projection),
          fov(fov),
          pos(pos),
          rot(rot),
          near(near),
          far(far),
          matrix(1),
          changed(true) {}

    Camera::~Camera() {}

    const glm::uvec2 & Camera::getScreenSize() const {
        return screenSize;
    }

    void Camera::setScreenSize(const glm::uvec2 & screen) {
        this->screenSize = screen;
    }

    float Camera::getFov() const {
        return fov;
    }

    void Camera::setFov(float fov) {
        this->fov = fov;
    }

    Camera::ProjectionMode Camera::getProjection() const {
        return projectionMode;
    }

    void Camera::setProjection(Camera::ProjectionMode mode) {
        projectionMode = mode;
    }

    const glm::vec3 & Camera::getPosition() const {
        return pos;
    }

    void Camera::setPosition(const glm::vec3 & pos) {
        this->pos = pos;
        changed = true;
    }

    const glm::vec3 & Camera::getRotation() const {
        return rot;
    }

    void Camera::setRotation(const glm::vec3 & rot) {
        this->rot = rot;
        changed = true;
    }

    void Camera::rotateDolly(const glm::vec3 & delta) {
        rot.x += delta.x;
        rot.y += delta.y;
        rot.x = glm::clamp(rot.x, rotXMin, rotXMax);
        changed = true;
    }

    void Camera::moveDolly(const glm::vec3 & delta) {
        float yRot = rot.y;

        float dz = delta.x * std::sin(yRot) + delta.z * std::cos(yRot);
        float dx = delta.x * std::cos(yRot) - delta.z * std::sin(yRot);

        float dy = delta.y;

        pos += glm::vec3(dx, dy, dz);
        changed = true;
    }

    glm::mat4 Camera::viewMatrix() const {
        if (changed) {
            auto rotX = glm::rotate(glm::mat4(1), rot.x, glm::vec3(1, 0, 0));
            auto rotY = glm::rotate(rotX, rot.y, glm::vec3(0, 1, 0));
            matrix = glm::translate(rotY, -pos);
            changed = false;
        }
        return matrix;
    }

    glm::mat4 Camera::projMatrix() const {
        float r = (float)screenSize.x / (float)screenSize.y;
        if (projectionMode == Perspective)
            return glm::perspective(glm::radians(fov), r, near, far);
        else
            return glm::ortho<float>(0, screenSize.x, screenSize.y, 0);
    }
}
