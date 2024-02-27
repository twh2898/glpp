#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace glpp::extra {

    /**
     * First person camera used to generate the projection and view matrices.
     */
    class Camera {
    public:
        using Ptr = shared_ptr<Camera>;
        using ConstPtr = const shared_ptr<Camera>;

        /**
         * Projection mode for projection matrix.
         */
        enum ProjectionMode {
            /// Perspective projection using fov
            Perspective,
            /// Orthographic projection
            Orthographic,
        };

    private:
        glm::uvec2 screenSize;
        ProjectionMode projectionMode;
        float fov;
        glm::vec3 pos;
        glm::vec3 rot;
        float near;
        float far;

        mutable glm::mat4 matrix;
        mutable bool changed;

    public:
        Camera();

        /**
         * Construct a new Camera with position (0, 0, 0) and rotation (0, 0)
         */
        Camera(const glm::uvec2 & screenSize,
               ProjectionMode projection = Perspective,
               float fov = 70.0f,
               const glm::vec3 & pos = glm::vec3(0),
               const glm::vec3 & rot = glm::vec3(0),
               float near = 0.1f,
               float far = 100.0f);

        /**
         * Default copy constructor.
         */
        Camera(const Camera & other) = default;

        /**
         * Default move constructor.
         */
        Camera(Camera && other) = default;

        /**
         * Default copy assign operator.
         */
        Camera & operator=(const Camera & other) = default;

        /**
         * Default move assign operator.
         */
        Camera & operator=(Camera && other) = default;

        /**
         * Destruct the camera.
         */
        virtual ~Camera();

        const glm::uvec2 & getScreenSize() const;

        /**
         * Tell the camera what the current screen size in pixels.
         * This is used when generating the projection matrix.
         *
         * @param screen the screen size in pixels
         */
        void setScreenSize(const glm::uvec2 & screen);

        /**
         * Get the current Field of View (FOV).
         *
         * @return the current FOV
         */
        float getFov() const;

        /**
         * Set the Field of View (FOV).
         *
         * @param fov the field of view in degrees of view
         */
        void setFov(float fov);

        /**
         * Get the current projection mode. Orthographic or Perspective.
         * The projection mode is used to generate the projection matrix.
         *
         * @returns the current perspective mode
         */
        ProjectionMode getProjection() const;

        /**
         * Set the projection mode. Orthographic or Perspective.
         * This is used to generate the projection matrix.
         *
         * @param mode the new projection mode
         */
        void setProjection(ProjectionMode mode);

        /**
         * Get the camera position.
         *
         * @return the position
         */
        const glm::vec3 & getPosition() const;

        /**
         * Set the camera position.
         *
         * @param pos the new position
         */
        void setPosition(const glm::vec3 & pos);

        /**
         * Get the euler camera rotation.
         *
         * @return the euler rotation
         */
        const glm::vec3 & getRotation() const;

        /**
         * Set the euler camera rotation.
         *
         * @param rot the euler rotation
         */
        void setRotation(const glm::vec3 & rot);

        /**
         * Rotate by the euler delta.
         *
         * @param delta the delta euler rotation
         */
        void rotateDolly(const glm::vec3 & delta);

        /**
         * Move the camera using the current rotation as the forward vector.
         *
         * @param delta the delta position
         */
        void moveDolly(const glm::vec3 & delta);

        /**
         * Get the view matrix
         *
         * @return the view matrix
         */
        glm::mat4 viewMatrix() const;

        /**
         * Get the projection matrix. This is generated from screen size and
         * the current field of view.
         *
         * @return the projection matrix
         */
        glm::mat4 projMatrix() const;
    };
}
