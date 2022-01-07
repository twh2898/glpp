#include "glpp/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <SFML/Graphics/Image.hpp>

namespace glpp {
    Texture::Texture(const unsigned char * data,
                     const glm::uvec2 & size,
                     size_t nrComponents,
                     GLint magFilter,
                     GLint minFilter,
                     GLint wrap,
                     bool mipmaps)
        : textureId(0),
          size(size),
          internal(RGBA),
          format(RGBA),
          type(GL_UNSIGNED_BYTE),
          samples(0),
          target(GL_TEXTURE_2D),
          minFilter(minFilter),
          magFilter(magFilter),
          wrap(wrap),
          mipmaps(mipmaps) {

        glGenTextures(1, &textureId);
        loadFrom(data, size, nrComponents);
    }

    Texture::Texture(const glm::uvec2 & size,
                     Format internal,
                     Format format,
                     GLenum type,
                     GLsizei samples,
                     GLint magFilter,
                     GLint minFilter,
                     GLint wrap,
                     bool mipmaps)
        : textureId(0),
          size(size),
          internal(internal),
          format(format),
          type(type),
          samples(samples),
          target(samples > 0 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
          minFilter(minFilter),
          magFilter(magFilter),
          wrap(wrap),
          mipmaps(mipmaps) {

        glGenTextures(1, &textureId);
        resize(size);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &textureId);
    }

    void Texture::loadFrom(const unsigned char * data,
                           const glm::uvec2 & size,
                           size_t nrComponents) {
        bind();

        this->size = size;
        if (nrComponents == 1)
            internal = Gray;
        else if (nrComponents == 3)
            internal = RGB;
        else if (nrComponents == 4)
            internal = RGBA;
        else
            throw TextureLoadException("Unsupported number of components");
        format = internal;
        type = GL_UNSIGNED_BYTE;
        samples = 0;
        target = GL_TEXTURE_2D;

        glTexImage2D(target, 0, internal, size.x, size.y, 0, format, type, data);

        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);

        if (mipmaps)
            glGenerateMipmap(target);
        unbind();
    }

    void Texture::loadFrom(const std::string & path) {
        int x, y, n;
        auto * data = stbi_load(path.c_str(), &x, &y, &n, 0);
        if (!data)
            throw TextureLoadException("Failed to load image from file");
        loadFrom(data, glm::uvec2(x, y), n);
    }

    GLuint Texture::getTextureId() const {
        return textureId;
    }

    const glm::uvec2 & Texture::getSize() const {
        return size;
    }

    void Texture::resize(const glm::uvec2 & size) {
        this->size = size;
        if (size.x > 0 && size.y > 0) {
            bind();
            if (samples > 0) {
                glTexImage2DMultisample(target, samples, internal, size.x,
                                        size.y, GL_TRUE);
            }
            else {
                glTexImage2D(target, 0, internal, size.x, size.y, 0, format,
                             type, NULL);

                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

                glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
                glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
            }
            unbind();
        }
    }

    void Texture::bind() const {
        glBindTexture(target, textureId);
    }

    void Texture::unbind() const {
        glBindTexture(target, 0);
    }
}
