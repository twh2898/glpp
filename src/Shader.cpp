#include "glpp/Shader.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace glpp {
    std::string shaderSource(const std::string & path) {
        std::ifstream is;
        is.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        is.open(path);

        std::string text((std::istreambuf_iterator<char>(is)),
                         std::istreambuf_iterator<char>());

        return text;
    }

    /**
     * Check if the shader compile step was successful.
     *
     * @param shader the shader to check
     *
     * @return was the shader compiled successfully
     */
    static bool compileSuccess(GLuint shader) {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        return success != GL_FALSE;
    }

    /**
     * Check if the shader link step was successful.
     *
     * @param program the shader to check
     *
     * @return was the shader linked successfully
     */
    static bool linkSuccess(GLuint program) {
        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        return success != GL_FALSE;
    }

    /**
     * Get the error message from the shader compile step.
     *
     * @param shader the shader to check
     *
     * @return a string with the shader compile error message
     */
    static std::string compileError(GLuint shader) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        std::vector<GLchar> errorLog(logSize);
        glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);

        return std::string(errorLog.begin(), errorLog.end());
    }

    /**
     * Get the error message from the shader link step.
     *
     * @param program the shader to check
     *
     * @return a string with the shader link error message
     */
    static std::string linkError(GLuint program) {
        GLint logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        std::vector<GLchar> errorLog(logSize);
        glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);

        return std::string(errorLog.begin(), errorLog.end());
    }

    /**
     * Compile a shader from source.
     *
     * @param shaderType GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @param shaderSource the source code for the shader
     *
     * @return the shader id
     *
     * @pre shaderSource must be null terminated
     */
    static GLuint compileShader(GLuint shaderType,
                                const std::string_view & shaderSource) {
        GLuint shader = glCreateShader(shaderType);
        const char * source = shaderSource.data();
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        if (!compileSuccess(shader)) {
            throw ShaderCompileException(shader);
        }
        return shader;
    }
}

namespace glpp {
    ShaderCompileException::ShaderCompileException(GLuint shader)
        : msg(compileError(shader)) {}

    const char * ShaderCompileException::what() const noexcept {
        return msg.c_str();
    }
}

namespace glpp {
    ShaderLinkException::ShaderLinkException(GLuint program)
        : msg(linkError(program)) {}

    const char * ShaderLinkException::what() const noexcept {
        return msg.c_str();
    }
}

namespace glpp {
    Shader::Shader() : program(glCreateProgram()) {}

    Shader::Shader(const std::string_view & vertexSource,
                   const std::string_view & fragmentSource) {
        GLuint vShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        glAttachShader(program, vShader);
        glAttachShader(program, fShader);

        glLinkProgram(program);

        glDetachShader(program, vShader);
        glDetachShader(program, fShader);
        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (!linkSuccess(program)) {
            throw ShaderLinkException(program);
        }
    }

    Shader::~Shader() {
        glDeleteProgram(program);
    }

    GLuint Shader::getProgram() const {
        return program;
    }

    void Shader::bind() const {
        glUseProgram(program);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    GLuint Shader::uniformLocation(const std::string & name) const {
        return glGetUniformLocation(program, name.c_str());
    }

    void Shader::setBool(const std::string & name, bool value) const {
        setBool(uniformLocation(name), value);
    }

    void Shader::setBool(GLuint location, bool value) const {
        glUniform1i(location, static_cast<int>(value));
    }

    void Shader::setInt(const std::string & name, int value) const {
        setInt(uniformLocation(name), value);
    }

    void Shader::setInt(GLuint location, int value) const {
        glUniform1i(location, value);
    }

    void Shader::setUInt(const std::string & name, unsigned int value) const {
        setUInt(uniformLocation(name), value);
    }

    void Shader::setUInt(GLuint location, unsigned int value) const {
        glUniform1ui(location, value);
    }

    void Shader::setFloat(const std::string & name, float value) const {
        setFloat(uniformLocation(name), value);
    }

    void Shader::setFloat(GLuint location, float value) const {
        glUniform1f(location, value);
    }

    void Shader::setVec2(const std::string & name, const glm::vec2 & value) const {
        setVec2(uniformLocation(name), value);
    }

    void Shader::setVec2(GLuint location, const glm::vec2 & value) const {
        glUniform2fv(location, 1, &value.x);
    }

    void Shader::setVec3(const std::string & name, const glm::vec3 & value) const {
        setVec3(uniformLocation(name), value);
    }

    void Shader::setVec3(GLuint location, const glm::vec3 & value) const {
        glUniform3fv(location, 1, &value.x);
    }

    void Shader::setVec4(const std::string & name, const glm::vec4 & value) const {
        setVec4(uniformLocation(name), value);
    }

    void Shader::setVec4(GLuint location, const glm::vec4 & value) const {
        glUniform4fv(location, 1, &value.x);
    }

    void Shader::setMat2(const std::string & name, const glm::mat2 & value) const {
        setMat2(uniformLocation(name), value);
    }

    void Shader::setMat2(GLuint location, const glm::mat2 & value) const {
        glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMat3(const std::string & name, const glm::mat3 & value) const {
        setMat3(uniformLocation(name), value);
    }

    void Shader::setMat3(GLuint location, const glm::mat3 & value) const {
        glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMat4(const std::string & name, const glm::mat4 & value) const {
        setMat4(uniformLocation(name), value);
    }

    void Shader::setMat4(GLuint location, const glm::mat4 & value) const {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

namespace glpp {
    static const std::string_view defaultVertexShaderSource = R"(
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

    static const std::string_view defaultFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform sampler2D gTexture;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    FragColor = texture(gTexture, FragTex);
})";

    Shader Shader::defaultShader() {
        Shader shader(defaultVertexShaderSource, defaultVertexShaderSource);
        return shader;
    }

    Shader fromPaths(const std::string & vertexPath,
                     const std::string & fragmentPath) {
        auto vertexSource = shaderSource(vertexPath);
        auto fragmentSource = shaderSource(fragmentPath);
        Shader shader(vertexSource, fragmentSource);
        return shader;
    }

    Shader Shader::fromFragmentPath(const std::string & path) {
        auto source = shaderSource(path);
        return fromFragmentSource(source);
    }
}
