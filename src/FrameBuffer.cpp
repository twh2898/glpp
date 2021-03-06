#include "glpp/FrameBuffer.hpp"

#include <stdexcept>

namespace glpp {
    RenderBuffer::RenderBuffer(const glm::uvec2 & size, GLenum internal)
        : internal(internal), size(size) {
        glGenRenderbuffers(1, &buffer);
        resize(size);
    }

    RenderBuffer::RenderBuffer(RenderBuffer && other)
        : buffer(other.buffer), internal(other.internal), size(other.size) {
        other.buffer = 0;
    }

    RenderBuffer & RenderBuffer::operator=(RenderBuffer && other) {
        buffer = other.buffer;
        other.buffer = 0;
        internal = other.internal;
        size = other.size;
        return *this;
    }

    RenderBuffer::~RenderBuffer() {
        if (buffer)
            glDeleteRenderbuffers(1, &buffer);
    }

    GLuint RenderBuffer::getBufferId() const {
        return buffer;
    }

    const glm::uvec2 & RenderBuffer::getSize() const {
        return size;
    }

    void RenderBuffer::resize(const glm::uvec2 & size) {
        this->size = size;
        bind();
        glRenderbufferStorage(GL_RENDERBUFFER, internal, size.x, size.y);
    }

    void RenderBuffer::bind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
    }

    void RenderBuffer::unbind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

namespace glpp {
    FrameBuffer::Attachment::Attachment(Texture * texture, GLenum attachment)
        : texture(texture), type(TEXTURE), attachment(attachment) {}

    FrameBuffer::Attachment::Attachment(RenderBuffer * buffer, GLenum attachment)
        : buffer(buffer), type(RENDER_BUFFER), attachment(attachment) {}

    void FrameBuffer::Attachment::resize(const glm::uvec2 & size) {
        if (type == TEXTURE)
            texture->resize(size);
        else
            buffer->resize(size);
    }
}

namespace glpp {
    FrameBuffer::FrameBuffer(const glm::uvec2 & size) : size(size) {
        glGenFramebuffers(1, &buffer);
        bind();
    }

    FrameBuffer::FrameBuffer(FrameBuffer && other)
        : buffer(other.buffer),
          attachments(std::move(other.attachments)),
          size(other.size) {
        other.buffer = 0;
    }

    FrameBuffer & FrameBuffer::operator=(FrameBuffer && other) {
        buffer = other.buffer;
        other.buffer = 0;
        attachments = std::move(other.attachments);
        size = other.size;
        return *this;
    }

    FrameBuffer::~FrameBuffer() {
        if (buffer)
            glDeleteFramebuffers(1, &buffer);
    }

    GLuint FrameBuffer::getBufferId() const {
        return buffer;
    }

    const std::vector<FrameBuffer::Attachment> & FrameBuffer::getAttachments() const {
        return attachments;
    }

    const glm::uvec2 & FrameBuffer::getSize() const {
        return size;
    }

    void FrameBuffer::resize(const glm::uvec2 & size) {
        this->size = size;
        for (auto & att : attachments) {
            att.resize(size);
        }
    }

    void FrameBuffer::blit(const FrameBuffer & source,
                           GLbitfield mask,
                           GLenum filter) const {
        source.bind(GL_READ_FRAMEBUFFER);
        bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, source.size.x, source.size.y, //
                          0, 0, size.x, size.y, //
                          mask, filter);
    }

    void FrameBuffer::bind(GLenum target) const {
        glBindFramebuffer(target, buffer);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    FrameBuffer & FrameBuffer::getDefault() {
        static FrameBuffer buffer(0);
        return buffer;
    }
}
