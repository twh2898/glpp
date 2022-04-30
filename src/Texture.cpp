#include "glpp/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace glpp {
    Texture::Texture(const unsigned char * data,
                     const glm::uvec2 & size,
                     size_t nrComponents,
                     Filter magFilter,
                     Filter minFilter,
                     Wrap wrap,
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
                     Filter magFilter,
                     Filter minFilter,
                     Wrap wrap,
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
        if (textureId)
            glDeleteTextures(1, &textureId);
    }

    Texture::Texture(Texture && other)
        : textureId(other.textureId),
          size(other.size),
          internal(other.internal),
          format(other.format),
          type(other.type),
          samples(other.samples),
          target(other.target),
          magFilter(other.magFilter),
          minFilter(other.minFilter),
          wrap(other.wrap),
          mipmaps(other.mipmaps) {
        other.textureId = 0;
    }

    Texture & Texture::operator=(Texture && other) {
        textureId = other.textureId;
        other.textureId = 0;
        size = other.size;
        internal = other.internal;
        format = other.format;
        type = other.type;
        samples = other.samples;
        target = other.target;
        magFilter = other.magFilter;
        minFilter = other.minFilter;
        wrap = other.wrap;
        mipmaps = other.mipmaps;
        return *this;
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

    GLuint Texture::getTextureId() const {
        return textureId;
    }

    GLsizei Texture::getSamples() const {
        return samples;
    }

    GLenum Texture::getTarget() const {
        return target;
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

    Texture Texture::fromPath(const std::string & path) {
        int x, y, n;
        auto * data = stbi_load(path.c_str(), &x, &y, &n, 0);
        if (!data)
            throw TextureLoadException("Failed to load image from file");
        return Texture(data, glm::uvec2(x, y), n);
    }
}
