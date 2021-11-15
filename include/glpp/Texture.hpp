#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <glm/glm.hpp>

namespace glpp {

    /**
     * Manages a single OpenGL texture.
     */
    class Texture {
        GLuint textureId;
        glm::uvec2 size;
        GLint internal;
        GLenum format;
        GLenum type;
        GLsizei samples;
        GLenum target;

        GLint magFilter, minFilter;
        GLint wrap;
        bool mipmaps;

    public:
        /**
         * Create a texture from an image.
         *
         * @param data the pixel data
         * @param size the image dimensions in pixesl
         * @param nrComponents the number of components for each pixel
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param mipmaps should mipmaps be generated
         */
        Texture(const unsigned char * data,
                const glm::uvec2 & size,
                size_t nrComponents,
                GLint magFilter = GL_LINEAR,
                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                GLint wrap = GL_REPEAT,
                bool mipmaps = true);

        /**
         * Create an empty texture of the specified size.
         *
         * Note: if samples is > 0, the filter, wrap and mipmaps arguments are
         * ignored.
         *
         * @param size the image size in pixels
         * @param internal the internal format like GL_RGBA16F, GL_RGBA,
         *                 GL_DEPTH_COMPONENT, etc.
         * @param format the format of pixel data
         * @param type the data type of pixel data
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param mipmaps should mipmaps be generated
         */
        Texture(const glm::uvec2 & size,
                GLint internal = GL_RGBA,
                GLenum format = GL_RGBA,
                GLenum type = GL_FLOAT,
                GLsizei samples = 0,
                GLint magFilter = GL_LINEAR,
                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                GLint wrap = GL_REPEAT,
                bool mipmaps = true);

        /// Free all opengl resources
        virtual ~Texture();

        /**
         * Default move constructor.
         */
        Texture(Texture && other) = default;

        /**
         * Default move assign operator.
         */
        Texture & operator=(Texture && other) = default;

        /**
         * Load the texture from an image, setting the size to match
         * image.getSize().
         *
         * @param data the pixel data
         * @param size the image dimensions in pixesl
         * @param nrComponents the number of components for each pixel
         */
        void loadFrom(const unsigned char * data,
                      const glm::uvec2 & size,
                      size_t nrComponents);

        /**
         * Get the OpenGL texture id.
         *
         * @return the texture id
         */
        GLuint getTextureId() const;

        /**
         * Get the texture size
         *
         * @return the texture size
         */
        const glm::uvec2 & getSize() const;

        /**
         * Resize the texture clearing all data.
         *
         * @param size the new texture size
         */
        void resize(const glm::uvec2 & size);

        /**
         * Bind the texture.
         */
        void bind() const;

        /**
         * Unbind the texture, effectively binding 0.
         */
        void unbind() const;
    };
}
