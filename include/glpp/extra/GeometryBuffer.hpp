#pragma once

#include "glpp/FrameBuffer.hpp"
#include "glpp/Shader.hpp"
#include "glpp/Texture.hpp"

namespace glpp::extra {
    class GeometryBuffer : public FrameBuffer {
        Texture diffuse;
        Texture normal;
        Texture position;
        Texture specular;
        RenderBuffer depth;

    public:
        using FrameBuffer::FrameBuffer;
        using FrameBuffer::operator=;

        GeometryBuffer(const glm::uvec2 & size);

        void bindTextures() const;

        static Shader getShader();
    };
}
