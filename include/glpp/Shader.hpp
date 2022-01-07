#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <stdexcept>
#include <string>
#include <string_view>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace glpp {

    class ShaderCompileException : public std::runtime_error {
    public:
        ShaderCompileException(GLuint shader);
    };

    class ShaderLinkException : public std::runtime_error {
    public:
        ShaderLinkException(GLuint program);
    };

    /**
     * Open a source file at path and read it's content.
     *
     * @param path the path to the source file
     *
     * @return a string that contains the file's contents
     */
    std::string shaderSource(const std::string & path);

    /**
     * Manages a single OpenGL shader.
     */
    class Shader {
        GLuint program;

    public:
        /**
         * Create a new Shader with no program.
         */
        Shader();

        /**
         * Create a new Shader with program from vertex and fragment source.
         *
         * @param vertexSource the vertex shader source
         * @param fragmentSource the fragment shader source
         *
         * @pre vertexSource and fragmentSource must be null terminated.
         */
        Shader(const std::string_view & vertexSource,
               const std::string_view & fragmentSource);

        /// Free OpenGL resources
        virtual ~Shader();

        /**
         * Default move constructor.
         */
        Shader(Shader && other) = default;

        /**
         * Default move assign operator.
         */
        Shader & operator=(Shader && other) = default;

        /**
         * Get the OpenGL shader program id.
         *
         * @return the shader program id
         */
        GLuint getProgram() const;

        /**
         * Bind the shader.
         */
        void bind() const;

        /**
         * Unbind the shader, effectively binding 0.
         */
        void unbind() const;

        /**
         * Get the uniform location for name in this shader.
         *
         * @param name the uniform name
         *
         * @return the uniform location
         */
        GLuint uniformLocation(const std::string & name) const;

        /**
         * Get the uniform location for name and set the value to integer 0 or 1.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setBool(const std::string & name, bool value) const;

        /**
         * Set the value to integer 0 or 1.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setBool(GLuint location, bool value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setInt(const std::string & name, int value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setInt(GLuint location, int value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setUInt(const std::string & name, unsigned int value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setUInt(GLuint location, unsigned int value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setFloat(const std::string & name, float value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setFloat(GLuint location, float value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec2(const std::string & name, const glm::vec2 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec2(GLuint location, const glm::vec2 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec3(const std::string & name, const glm::vec3 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec3(GLuint location, const glm::vec3 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setVec4(const std::string & name, const glm::vec4 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setVec4(GLuint location, const glm::vec4 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat2(const std::string & name, const glm::mat2 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat2(GLuint location, const glm::mat2 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat3(const std::string & name, const glm::mat3 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat3(GLuint location, const glm::mat3 & value) const;

        /**
         * Get the uniform location for name and set the value.
         *
         * @param name the uniform name
         * @param value the value to set
         */
        void setMat4(const std::string & name, const glm::mat4 & value) const;

        /**
         * Set the value.
         *
         * @param location the uniform location
         * @param value the value to set
         */
        void setMat4(GLuint location, const glm::mat4 & value) const;

        /**
         * Load the default shader from the internal source.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * @return the default shader
         */
        static Shader defaultShader();

        /**
         * Load a shader using the default vertex shader and a fragment shader.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * Out to fragment shader:
         * - FragPos: vec3
         * - FragNorm: vec3
         * - FragTex: vec2
         *
         * @param source the fragment shader source
         *
         * @pre source must be null terminated.
         *
         * @return the shader
         */
        static Shader fromFragmentSource(const std::string_view & source);

        /**
         * Load a shader using a vertex and fragment shader path.
         *
         * @param vertexPath the path to the vertex shader source
         * @param fragmentPath the path to the fragment shader source
         *
         * @return the shader
         */
        static Shader fromPaths(const std::string & vertexPath,
                                const std::string & fragmentPath);

        /**
         * Load a shader using the default vertex shader and a fragment shader.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * Out to fragment shader:
         * - FragPos: vec3
         * - FragNorm: vec3
         * - FragTex: vec2
         *
         * @param path the path to the fragment shader source
         *
         * @return the shader
         */
        static Shader fromFragmentPath(const std::string & path);
    };
}
