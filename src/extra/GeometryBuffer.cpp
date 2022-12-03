#include "glpp/extra/GeometryBuffer.hpp"

#include <string_view>

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
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(mvp * vec4(aPos, 1.0));
    FragNorm = aNorm;
    FragTex = aTex;
})";

    static const string_view geometryFragmentShaderSource = R"(
#version 330 core
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

uniform sampler2D gTexture;

out vec4 oDiffuse;
out vec4 oNormal;
out vec4 oPosition;
out vec4 oSpecular;

void main() {
    oDiffuse = texture(gTexture, FragTex);
    oNormal = vec4(FragNorm, 1.0);
    oPosition = vec4(FragPos, 1.0);
    oSpecular = vec4(1.0);
})";

    Shader GeometryBuffer::getShader() {
        return Shader(geometryVertexShaderSource, geometryFragmentShaderSource);
    }
}
