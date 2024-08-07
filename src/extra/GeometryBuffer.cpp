#include "glpp/extra/GeometryBuffer.hpp"

#include <string_view>

namespace glpp::extra {
    using std::make_shared;

    GeometryBuffer::GeometryBuffer(const glm::uvec2 & size, GLsizei samples)
        : FrameBuffer(size),
          diffuse(make_shared<Texture>(size,
                                       Texture::RGB,
                                       Texture::RGB,
                                       GL_FLOAT,
                                       samples,
                                       Texture::Linear,
                                       Texture::Linear,
                                       Texture::Clamp,
                                       false)),
          normal(make_shared<Texture>(size,
                                      (Texture::Format)GL_RGB16F,
                                      Texture::RGB,
                                      GL_FLOAT,
                                      samples,
                                      Texture::Linear,
                                      Texture::Linear,
                                      Texture::Clamp,
                                      false)),
          position(make_shared<Texture>(size,
                                        (Texture::Format)GL_RGB16F,
                                        Texture::RGB,
                                        GL_FLOAT,
                                        samples,
                                        Texture::Linear,
                                        Texture::Linear,
                                        Texture::Clamp,
                                        false)),
          specular(make_shared<Texture>(size,
                                        (Texture::Format)GL_R16F,
                                        Texture::Gray,
                                        GL_FLOAT,
                                        samples,
                                        Texture::Linear,
                                        Texture::Linear,
                                        Texture::Clamp,
                                        false)),
          depth(make_shared<RenderBuffer>(size, GL_DEPTH24_STENCIL8, samples)) {

        attach(diffuse, GL_COLOR_ATTACHMENT0);
        attach(normal, GL_COLOR_ATTACHMENT1);
        attach(position, GL_COLOR_ATTACHMENT2);
        attach(specular, GL_COLOR_ATTACHMENT3);
        attach(depth, GL_DEPTH_STENCIL_ATTACHMENT);
    }

    GeometryBuffer::~GeometryBuffer() {}

    void GeometryBuffer::bindTextures() const {
        diffuse->bind(0);
        normal->bind(1);
        position->bind(2);
        specular->bind(3);
    }
}

namespace glpp::extra {
    using std::string_view;

    static const string_view geometryVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
out vec3 FragPos;
out vec3 FragNorm;
out vec2 FragTex;
uniform mat4 model;
uniform mat4 vp;
void main() {
    mat4 mvp = vp * model;
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragNorm = aNorm;
    FragTex = aTex;
})";

    static const string_view geometryFragmentShaderSource = R"(
#version 330 core
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

uniform sampler2D gTexture;

layout (location = 0) out vec3 oDiffuse;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec3 oPosition;
layout (location = 3) out float oSpecular;

void main() {
    oDiffuse = texture(gTexture, FragTex).xyz;
    oNormal = normalize(FragNorm);
    oPosition = FragPos;
    oSpecular = 1.0;
})";

    Shader & GeometryBuffer::getShader() {
        static Shader shader(geometryVertexShaderSource,
                             geometryFragmentShaderSource);
        return shader;
    }
}
