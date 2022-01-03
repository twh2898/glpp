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
        GLuint vaa;
        GLint size;
        GLenum type;
        bool normalized;
        GLsizei stride;
        const void * pointer;

        /**
         * Create a new Attribute for an index vaa.
         *
         * @param vaa the vertex attribute index
         * @param size the number of values per vertex
         * @param type the opengl value type
         * @param normalized are the values normalized
         * @param stride the size in bytes of each value
         * @param pointer pointer to the data or offset within the element
         */
        Attribute(GLuint vaa,
                  GLint size,
                  GLenum type,
                  bool normalized,
                  GLsizei stride,
                  const void * pointer = nullptr);

        void enable() const;

        void disable() const;
    };

    /**
     * A single array buffer.
     */
    class Buffer {
    public:
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

    protected:
        GLuint vbo;
        Usage usage;
        std::vector<Attribute> attributes;

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
        Buffer(const std::vector<Attribute> & attributes,
               Usage usage = Usage::Static);

        Buffer(std::vector<Attribute> && attributes, Usage usage = Usage::Static);

        virtual ~Buffer();

        virtual const void * data() const = 0;
        virtual size_t size() const = 0;
        virtual size_t count() const = 0;

        /**
         * Enable all attributes.
         */
        void enable() const;

        /**
         * Disable all attributes
         */
        void disable() const;

        void bufferData();

        void bind() const;

        void unbind() const;

        void draw(Mode mode) const;
    };

    template<typename T>
    struct VectorBufferBase : public Buffer {
        using element_type = T;

        std::vector<element_type> buff;

        using Buffer::Buffer;

        const void * data() const override {
            return buff.data();
        }

        size_t size() const override {
            return buff.size() * sizeof(element_type);
        }

        size_t count() const override {
            return buff.size();
        }

        /**
         * Load buffer with data from buff.
         *
         * @param buff the data to send to the buffer
         */
        void loadFromPoints(const std::vector<element_type> & buff) {
            this->buff = buff;
            bufferData();
        }
    };

    struct PositionBuffer : public VectorBufferBase<glm::vec3> {
        const std::vector<Attribute> attributes {
            Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

        PositionBuffer(GLuint vaa, Usage usage = Usage::Static)
            : VectorBufferBase(attributes, usage) {}
    };

    struct ColorBuffer : public VectorBufferBase<glm::vec3> {
        const std::vector<Attribute> attributes {
            Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

        ColorBuffer(GLuint vaa, Usage usage = Usage::Static)
            : VectorBufferBase(attributes, usage) {}
    };

    struct NormalBuffer : public VectorBufferBase<glm::vec3> {
        const std::vector<Attribute> attributes {
            Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

        NormalBuffer(GLuint vaa, Usage usage = Usage::Static)
            : VectorBufferBase(attributes, usage) {}
    };

    struct TexCoordBuffer : public VectorBufferBase<glm::vec2> {
        const std::vector<Attribute> attributes {
            Attribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(element_type), 0)};

        TexCoordBuffer(GLuint vaa, Usage usage = Usage::Static)
            : VectorBufferBase(attributes, usage) {}
    };

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

    struct TextureVertexBuffer : public VectorBufferBase<Vertex> {
        const std::vector<Attribute> attributes {
            Attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(element_type), 0),
            Attribute(0,
                      3,
                      GL_FLOAT,
                      GL_FALSE,
                      sizeof(element_type),
                      (void *)(3 * sizeof(float))),
            Attribute(0,
                      2,
                      GL_FLOAT,
                      GL_FALSE,
                      sizeof(element_type),
                      (void *)(6 * sizeof(float)))};

        TextureVertexBuffer(Usage usage = Usage::Static)
            : VectorBufferBase(attributes, usage) {}
    };

    void draw_array(const std::vector<Vertex> & vertices, GLenum mode);

    void draw_quad(const glm::vec2 & pos, const glm::vec2 & size);

    /**
     * Manages a single vertex buffered object and vertex array object.
     */
    class BufferArray {
    public:
        using Mode = Buffer::Mode;

    private:
        GLuint vao;
        size_t nPoints;
        Mode mode;
        std::vector<std::shared_ptr<Buffer>> buffers;

    public:
        /**
         * Create a new VBO with mode and usage.
         *
         * @param mode the OpenGL draw mode
         * @param usage the OpenGL buffer usage
         */
        BufferArray(const std::vector<std::shared_ptr<Buffer>> & buffers,
                    Mode mode = Mode::Triangles);

        /**
         * Create a new VBO with mode and usage.
         *
         * @param mode the OpenGL draw mode
         * @param usage the OpenGL buffer usage
         */
        BufferArray(std::vector<std::shared_ptr<Buffer>> && buffers,
                    Mode mode = Mode::Triangles);

        /**
         * Free OpenGL buffers.
         */
        virtual ~BufferArray();

        /**
         * Default move constructor.
         */
        BufferArray(BufferArray && other) = default;

        /**
         * Default move assign operator.
         */
        BufferArray & operator=(BufferArray && other) = default;

        /**
         * Get the OpenGL vao id.
         *
         * @return the vao id
         */
        GLuint getId() const;

        /**
         * Get the number of points.
         *
         * @return the number of points
         */
        size_t size() const;

        /**
         * Get the OpenGL draw mode.
         *
         * @return the VBO::Mode
         */
        Mode getMode() const;

        /**
         * Set the OpenGL draw mode.
         *
         * @param mode the new mode
         */
        void setMode(Mode mode);

        /**
         * Bind the vao, then draw, then unbind the vao.
         */
        void draw() const;

        void bind() const;

        void unbind() const;
    };
}
