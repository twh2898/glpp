#pragma once

#include <glm/glm.hpp>

#include "glpp/Shader.hpp"

namespace glpp::extra {
    class Drawable {
    public:
        /**
         * Draw without binding shader or applying any transforms.
         */
        virtual void draw() const = 0;

        /**
         * Bind the shader and apply transform to the mvp uniform.
         *
         * @param transform
         */
        virtual void draw(const glm::mat4 & transform) const = 0;

        static Shader & defaultShader();
    };
}
