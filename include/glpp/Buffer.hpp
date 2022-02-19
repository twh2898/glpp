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
        Usage usage;
        std::unique_ptr<Attribute> attrib;

    public:
        /**
         * Create a new VBO.
         *
         * @param vaa the Vertex Attribute index
         * @param usage the OpenGL buffer usage
         *
         * After creating the vertex array attribute vaa will be enabled and the
         * new vbo will be bound.
         */
        Buffer(Target target = Array, Usage usage = Static);

        Buffer(const Attribute & attrib, Target target = Array, Usage usage = Static);

        Buffer(Buffer && other);

        Buffer(const Buffer &) = delete;
        Buffer & operator=(const Buffer &) = delete;
        Buffer & operator=(Buffer &&) = delete;

        ~Buffer();

        Target getTarget() const;

        GLuint getId() const;

        Usage getUsage() const;

        void setUsage(Usage usage);

        Attribute * getAttribute() const;

        void bind() const;

        void unbind() const;

        void bufferData(GLsizeiptr size, const void * data);

        void bufferSubData(GLintptr offset, GLsizeiptr size, const void * data);
    };

    class BufferArray {
        GLuint array;
        std::vector<Buffer> buffers;
        std::unique_ptr<Buffer> elementBuffer;

    public:
        using Mode = Buffer::Mode;

        BufferArray();

        BufferArray(std::vector<Buffer> && buffers);

        BufferArray(const std::vector<Attribute> & attributes);

        BufferArray(const BufferArray &) = delete;
        BufferArray(BufferArray &&) = delete;
        BufferArray & operator=(const BufferArray &) = delete;
        BufferArray & operator=(BufferArray &&) = delete;

        ~BufferArray();

        GLuint getId() const;

        /**
         * The number of buffers not including the optional index buffer.
         */
        std::size_t count() const;

        std::vector<Buffer> & getBuffers();

        void bind() const;

        void unbind() const;

        void bufferData(size_t index, GLsizeiptr size, const void * data);

        void bufferSubData(size_t index,
                           GLintptr offset,
                           GLsizeiptr size,
                           const void * data);

        void bufferElements(GLsizeiptr size, const void * data);

        void drawArrays(GLint first, GLsizei count, Mode mode = Mode::Triangles) const;

        void drawElements(GLsizei count,
                          GLenum type = GL_UNSIGNED_INT,
                          Mode mode = Mode::Triangles,
                          const void * indices = nullptr) const;
    };
}
