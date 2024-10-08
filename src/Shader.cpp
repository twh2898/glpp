#include "glpp/Shader.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace glpp {
    using std::vector;

    string shaderSource(const string & path) {
        std::ifstream is;
        is.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        is.open(path);

        string text((std::istreambuf_iterator<char>(is)),
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
    static string compileError(GLuint shader) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        vector<GLchar> errorLog(logSize);
        glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);

        return string(errorLog.begin(), errorLog.end());
    }

    /**
     * Get the error message from the shader link step.
     *
     * @param program the shader to check
     *
     * @return a string with the shader link error message
     */
    static string linkError(GLuint program) {
        GLint logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        vector<GLchar> errorLog(logSize);
        glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);

        return string(errorLog.begin(), errorLog.end());
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
                                const string_view & shaderSource) {
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
        : std::runtime_error(compileError(shader)) {}
}

namespace glpp {
    ShaderLinkException::ShaderLinkException(GLuint program)
        : std::runtime_error(linkError(program)) {}
}

namespace glpp {
    Uniform::Uniform(GLuint location) : location(location) {}

    GLuint Uniform::getLocation() const {
        return location;
    }

    bool Uniform::exists() const {
        return location != -1;
    }

    void Uniform::setBool(bool value) const {
        glUniform1i(location, static_cast<int>(value));
    }

    void Uniform::setInt(int value) const {
        glUniform1i(location, value);
    }

    void Uniform::setUInt(unsigned int value) const {
        glUniform1ui(location, value);
    }

    void Uniform::setFloat(float value) const {
        glUniform1f(location, value);
    }

    void Uniform::setVec2(const glm::vec2 & value) const {
        glUniform2fv(location, 1, &value.x);
    }

    void Uniform::setVec3(const glm::vec3 & value) const {
        glUniform3fv(location, 1, &value.x);
    }

    void Uniform::setVec4(const glm::vec4 & value) const {
        glUniform4fv(location, 1, &value.x);
    }

    void Uniform::setMat2(const glm::mat2 & value) const {
        glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
    }

    void Uniform::setMat3(const glm::mat3 & value) const {
        glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
    }

    void Uniform::setMat4(const glm::mat4 & value) const {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

namespace glpp {
    Shader::Shader(const string_view & vertexSource,
                   const string_view & fragmentSource) {
        GLuint vShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        program = glCreateProgram();

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

    Shader::Shader(Shader && other) : program(other.program) {
        other.program = 0;
    }

    Shader & Shader::operator=(Shader && other) {
        program = other.program;
        other.program = 0;
        return *this;
    }

    Shader::~Shader() {
        if (program)
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

    Uniform Shader::uniform(const char * name) const {
        GLuint location = glGetUniformLocation(program, name);
        return Uniform(location);
    }
}

namespace glpp {
    static const string_view defaultVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
out vec3 FragPos;
out vec3 FragNorm;
out vec2 FragTex;
uniform mat4 vp;
uniform mat4 model;
void main() {
    mat4 mvp = vp * model;
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragNorm = aNorm;
    FragTex = aTex;
})";

    static const string_view defaultFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform sampler2D gTexture;
in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
void main() {
    FragColor = texture(gTexture, FragTex);
})";

    [[deprecated("A default shader will be provided in a different form, if at all, in future releases")]]
    Shader & Shader::defaultShader() {
        static Shader shader(defaultVertexShaderSource,
                             defaultFragmentShaderSource);
        return shader;
    }

    Shader Shader::fromFragmentSource(const string_view & fragmentSource) {
        return Shader(defaultVertexShaderSource, fragmentSource);
    }

    Shader Shader::fromPaths(const string & vertexPath,
                             const string & fragmentPath) {
        auto vertexSource = shaderSource(vertexPath);
        auto fragmentSource = shaderSource(fragmentPath);
        return Shader(vertexSource, fragmentSource);
    }

    Shader Shader::fromFragmentPath(const string & path) {
        auto source = shaderSource(path);
        return fromFragmentSource(source);
    }
}
