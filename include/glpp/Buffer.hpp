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

    // template<typename T>
    // class VectorBufferBase : public Buffer {
    // public:
    //     using element_type = T;

    // private:
    //     std::vector<element_type> buff;

    // public:
    //     using Buffer::Buffer;

    //     const void * data() const override {
    //         return buff.data();
    //     }

    //     size_t size() const override {
    //         return buff.size() * sizeof(element_type);
    //     }

    //     size_t count() const override {
    //         return buff.size();
    //     }

    //     /**
    //      * Load buffer with data from buff.
    //      *
    //      * @param buff the data to send to the buffer
    //      */
    //     void loadFromPoints(const std::vector<element_type> & buff) {
    //         this->buff = buff;
    //         bufferData();
    //     }
    // };

    // struct PositionBuffer : public VectorBufferBase<glm::vec3> {
    //     const std::vector<Attribute> attributes {
    //         Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

    //     PositionBuffer(Usage usage = Usage::Static)
    //         : VectorBufferBase(attributes, usage) {}
    // };

    // struct ColorBuffer : public VectorBufferBase<glm::vec3> {
    //     const std::vector<Attribute> attributes {
    //         Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

    //     ColorBuffer(Usage usage = Usage::Static)
    //         : VectorBufferBase(attributes, usage) {}
    // };

    // struct NormalBuffer : public VectorBufferBase<glm::vec3> {
    //     const std::vector<Attribute> attributes {
    //         Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

    //     NormalBuffer(Usage usage = Usage::Static)
    //         : VectorBufferBase(attributes, usage) {}
    // };

    // struct TexCoordBuffer : public VectorBufferBase<glm::vec2> {
    //     const std::vector<Attribute> attributes {
    //         Attribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

    //     TexCoordBuffer(Usage usage = Usage::Static)
    //         : VectorBufferBase(attributes, usage) {}
    // };

    /**
     * A single point in the format accepted by VBO, Mesh and Model.
     */
    struct Vertex {
        /// The position
        glm::vec3 pos;
        /// The normal
        glm::vec3 norm;
        /// The UV texture coordinate
        glm::vec2 uv;

        /**
         * Create a new vertex with all fields set to 0.
         */
        Vertex();

        /**
         * Create a new Vertex and set it fields to pos, norm and uv.
         *
         * @param pos the position
         * @param norm the normal
         * @param uv the texture coordinate
         */
        Vertex(const glm::vec3 & pos, const glm::vec3 & norm, const glm::vec2 & uv);

        /// Default copy constructor
        Vertex(const Vertex & other) = default;

        /// Default move constructor
        Vertex(Vertex && other) = default;

        /// Default copy assign operator
        Vertex & operator=(const Vertex & other) = default;

        /// Default move assign operator
        Vertex & operator=(Vertex && other) = default;

        /**
         * Add the fields element wise of two Vertex objects.
         *
         * @param other the Vertex to add to
         *
         * @return a new Vertex with the sum of both objects.
         */
        Vertex operator+(const Vertex & other) const;

        /**
         * Add offset to just the position field.
         *
         * @param offset the offset to add to pos
         *
         * @return a new Vertex with the sum of pos and offset
         */
        Vertex operator+(const glm::vec3 & offset) const;

        /**
         * Subtract the fields element wise of two Vertex objects.
         *
         * @param other the Vertex to subtract from this
         *
         * @return a new Vertex with the difference of both objects.
         */
        Vertex operator-(const Vertex & other) const;

        /**
         * Add the fields element wise of other to the fields of this.
         *
         * @param other the Vertex to add to this
         *
         * @return a reference to this
         */
        Vertex & operator+=(const Vertex & other);

        /**
         * Subtract the fields element wise of other from the fields of this.
         *
         * @param other the Vertex to subtract from this
         *
         * @return a reference to this
         */
        Vertex & operator-=(const Vertex & other);
    };

    // struct TextureVertexBuffer : public VectorBufferBase<Vertex> {
    //     const std::vector<Attribute> attributes {
    //         Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0),
    //         Attribute(0,
    //                   3,
    //                   GL_FLOAT,
    //                   GL_FALSE,
    //                   sizeof(element_type),
    //                   (void *)(3 * sizeof(float))),
    //         Attribute(0,
    //                   2,
    //                   GL_FLOAT,
    //                   GL_FALSE,
    //                   sizeof(element_type),
    //                   (void *)(6 * sizeof(float)))};

    //     TextureVertexBuffer(Usage usage = Usage::Static)
    //         : VectorBufferBase(attributes, usage) {}
    // };

    void draw_array(const std::vector<Vertex> & vertices, GLenum mode);

    void draw_quad(const glm::vec2 & pos, const glm::vec2 & size);
}
