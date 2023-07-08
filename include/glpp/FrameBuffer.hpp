#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <vector>

#include "Texture.hpp"

namespace glpp {
    using std::vector;

    class RenderBuffer {
        GLuint buffer;
        GLenum internal;
        GLsizei samples;
        glm::uvec2 size;

    public:
        RenderBuffer(const glm::uvec2 & size,
                     GLenum internal = GL_DEPTH24_STENCIL8,
                     GLsizei samples = 0);

        RenderBuffer(RenderBuffer && other);

        RenderBuffer & operator=(RenderBuffer && other);

        RenderBuffer(const RenderBuffer &) = delete;
        RenderBuffer & operator=(const RenderBuffer &) = delete;

        ~RenderBuffer();

        GLuint getBufferId() const;
        
        GLsizei getSamples() const;

        const glm::uvec2 & getSize() const;

        void resize(const glm::uvec2 & size);

        void bind() const;

        void unbind() const;
    };

    /**
     * Manage a single frame buffer object.
     */
    class FrameBuffer {
    public:
        struct Attachment {
            enum Type {
                TEXTURE,
                RENDER_BUFFER,
            };

            union {
                Texture * texture;
                RenderBuffer * buffer;
            };
            Type type;
            GLenum attachment;

            Attachment(Texture * texture, GLenum attachment);

            Attachment(RenderBuffer * buffer, GLenum attachment);

            void resize(const glm::uvec2 & size);
        };

    private:
        GLuint buffer;
        vector<Attachment> attachments;
        glm::uvec2 size;

        FrameBuffer(GLuint buffer) : buffer(buffer) {}

    public:
        FrameBuffer(const glm::uvec2 & size);

        FrameBuffer(FrameBuffer && other);

        FrameBuffer & operator=(FrameBuffer && other);

        FrameBuffer(const FrameBuffer &) = delete;
        FrameBuffer & operator=(const FrameBuffer &) = delete;

        /**
         * Destruct the FrameBuffer.
         */
        virtual ~FrameBuffer();

        bool isComplete() const;

        /**
         * Get the FrameBuffer id.
         *
         * @return the opengl id
         */
        GLuint getBufferId() const;

        void attach(Texture * texture, GLenum attachment = GL_COLOR_ATTACHMENT0);

        void attach(RenderBuffer * buffer,
                    GLenum attachment = GL_DEPTH_STENCIL_ATTACHMENT);

        /**
         * Get the attachments.
         *
         * @return the attachments
         */
        const vector<Attachment> & getAttachments() const;

        /**
         * Get the current size in pixels.
         *
         * @return the current size
         */
        const glm::uvec2 & getSize() const;

        /**
         * Set the size in pixels. This will also resize each
         * FrameBufferTexture under this FrameBuffer.
         *
         * @param size the new size in pixels
         */
        void resize(const glm::uvec2 & size);

        /**
         * Call glBlitFramebuffer with the FrameBuffer as the source and dest
         * as the destination. Use bitfield to select color, depth or stencil.
         */
        void blit(const FrameBuffer & source,
                  GLbitfield mask = GL_COLOR_BUFFER_BIT,
                  GLenum filter = GL_NEAREST) const;

        /**
         * Call glBlitFramebuffer with the FrameBuffer as the source and dest
         * as the destination. Use bitfield to select color, depth or stencil.
         */
        void blit(const FrameBuffer & source,
                  GLint sx0,
                  GLint sy0,
                  GLint sx1,
                  GLint sy1,
                  GLint dx0,
                  GLint dy0,
                  GLint dx1,
                  GLint dy1,
                  GLbitfield mask = GL_COLOR_BUFFER_BIT,
                  GLenum filter = GL_NEAREST) const;

        /**
         * Bind the FrameBuffer. All draw calls after this will be sent to the
         * FrameBuffer.
         */
        void bind(GLenum target = GL_FRAMEBUFFER) const;

        /**
         * Set the viewport ot the curren size.
         */
        void setViewport() const;

        /**
         * Unbind this FrameBuffer, effectively binding the default FrameBuffer.
         */
        static void unbind();

        /**
         * Call glClear with Color, Depth and Stencil buffer bits.
         */
        static void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
                                            | GL_STENCIL_BUFFER_BIT);

        static FrameBuffer & getDefault();
    };
}
