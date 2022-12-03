#include "glpp/extra/GeometryBuffer.hpp"

namespace glpp::extra {
    GeometryBuffer::GeometryBuffer(const glm::uvec2 & size)
        : FrameBuffer(size),
          diffuse(size),
          normal(size),
          position(size),
          specular(size),
          depth(size, GL_DEPTH24_STENCIL8) {
        attach(&diffuse, GL_COLOR_ATTACHMENT0);
        attach(&normal, GL_COLOR_ATTACHMENT1);
        attach(&position, GL_COLOR_ATTACHMENT2);
        attach(&specular, GL_COLOR_ATTACHMENT3);
        attach(&depth, GL_DEPTH_STENCIL_ATTACHMENT);
    }

    void GeometryBuffer::bindTextures() const {
        diffuse.bind(0);
        normal.bind(1);
        position.bind(2);
        specular.bind(3);
    }
}
