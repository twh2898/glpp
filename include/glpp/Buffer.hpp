#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace glpp {

    /**
     * Describes a vertex array attribute for a Buffer.
     */
    struct Attribute {
        GLuint index;
        GLint size;
        GLenum type;
        bool normalized;
        GLsizei stride;
        const void * pointer;
        GLuint divisor = 0;

        void enable() const;

        void disable() const;
    };

    /**
     * A single array buffer.
     */
    class Buffer {
    public:
        enum Target {
            Array = GL_ARRAY_BUFFER,
            Index = GL_ELEMENT_ARRAY_BUFFER,
        };

        /**
         * OpenGL buffer usage.
         */
        enum Usage {
            /// Data will be buffered once and used multiple times
            Static = GL_STATIC_DRAW,
            /// Data will be buffered and used a few times
            Stream = GL_STREAM_DRAW,
            /// Data will be buffered and used once
            Dynamic = GL_DYNAMIC_DRAW,
        };

        /**
         * OpenGL Draw mode.
         */
        enum Mode {
            Lines = GL_LINES,
            LineStrip = GL_LINE_STRIP,
            LineLoop = GL_LINE_LOOP,

            Triangles = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
        };

    private:
        Target target;
        GLuint buffer;

    public:
        /**
         * Create a new VBO.
         *
         * @param target the target buffer
         *
         * After creating the vertex array attribute vaa will be enabled and the
         * new vbo will be bound.
         */
        Buffer(Target target = Array);

        Buffer(Buffer && other);

        Buffer & operator=(Buffer && other);

        Buffer(const Buffer &) = delete;
        Buffer & operator=(const Buffer &) = delete;

        ~Buffer();

        Target getTarget() const;

        GLuint getBufferId() const;

        void bind() const;

        void unbind() const;

        void bufferData(GLsizeiptr size, const void * data, Usage usage = Static);

        void bufferSubData(GLintptr offset, GLsizeiptr size, const void * data);
    };

    struct AttributedBuffer {
        std::vector<Attribute> attrib;
        Buffer buffer;

        using Usage = Buffer::Usage;

        AttributedBuffer(const std::vector<Attribute> & attrib, Buffer && buffer);

        AttributedBuffer(AttributedBuffer && other);

        AttributedBuffer & operator=(AttributedBuffer && other);

        AttributedBuffer(const AttributedBuffer &) = delete;
        AttributedBuffer & operator=(const AttributedBuffer &) = delete;

        void bufferData(GLsizeiptr size,
                        const void * data,
                        Usage usage = Usage::Static);

        inline void bufferSubData(GLintptr offset, GLsizeiptr size, const void * data) {
            buffer.bufferSubData(offset, size, data);
        }
    };

    class BufferArray {
        GLuint array;
        std::vector<AttributedBuffer> buffers;
        std::unique_ptr<Buffer> elementBuffer;

    public:
        using Usage = Buffer::Usage;
        using Mode = Buffer::Mode;

        BufferArray();

        BufferArray(const std::vector<std::vector<Attribute>> & attributes);

        BufferArray(std::vector<Buffer> && buffers);

        BufferArray(BufferArray && other);

        BufferArray & operator=(BufferArray && other);

        BufferArray(const BufferArray &) = delete;
        BufferArray & operator=(const BufferArray &) = delete;

        ~BufferArray();

        GLuint getArrayId() const;

        /**
         * The number of buffers not including the optional index buffer.
         */
        std::size_t size() const;

        const std::vector<AttributedBuffer> & getBuffers() const;

        std::vector<AttributedBuffer> & getBuffers();

        void bind() const;

        void unbind() const;

        void bufferData(size_t index,
                        GLsizeiptr size,
                        const void * data,
                        Usage usage = Usage::Static);

        void bufferSubData(size_t index,
                           GLintptr offset,
                           GLsizeiptr size,
                           const void * data);

        void bufferElements(GLsizeiptr size,
                            const void * data,
                            Usage usage = Usage::Static);

        void drawArrays(Mode mode, GLint first, GLsizei count) const;


        void drawArraysInstanced(Mode mode,
                                 GLint first,
                                 GLsizei count,
                                 GLsizei primcount) const;

        void drawElements(Mode mode,
                          GLsizei count,
                          GLenum type,
                          const void * indices) const;


        void drawElementsInstanced(Mode mode,
                                   GLsizei count,
                                   GLenum type,
                                   const void * indices,
                                   GLsizei primcount) const;
    };
}
