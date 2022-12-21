#pragma once

#include <glm/glm.hpp>

#include "glpp/FrameBuffer.hpp"
#include "glpp/Shader.hpp"
#include "glpp/Texture.hpp"
#include "glpp/extra/Vertex.hpp"

namespace glpp::extra {
    using glm::uvec2;

    class GeometryBuffer : public FrameBuffer {
    public:
        Texture diffuse;
        Texture normal;
        Texture position;
        Texture specular;
        RenderBuffer depth;

        // TODO: implement these as public, remember to move textures
        using FrameBuffer::FrameBuffer;
        using FrameBuffer::operator=;

        // GeometryBuffer should manage these
        using FrameBuffer::attach;

    public:
        GeometryBuffer(const uvec2 & size, GLsizei samples = 0);

        void bindTextures() const;

        static Shader & getShader();
    };
}
