#pragma once

#include "glpp/Buffer.hpp"

namespace glpp::extra {
    template<typename T>
    class VectorBufferBase : public Buffer {
    public:
        using element_type = T;

    private:
        std::vector<element_type> buff;

        const size_t element_size = sizeof(element_type);

        inline void bufferData() {
            bufferData(element_size * this->buff.size(), this->buff.data());
        }

    public:
        VectorBufferBase(Target target = Array, Usage usage = Static)
            : Buffer(target, usage) {}

        VectorBufferBase(const Attribute & attrib,
                         Target target = Array,
                         Usage usage = Static)
            : Buffer(attrib, target, usage) {}

        VectorBufferBase(const std::vector<element_type> & buff,
                         Target target = Array,
                         Usage usage = Static)
            : Buffer(target, usage), buff(buff) {}

        VectorBufferBase(const std::vector<element_type> & buff,
                         const Attribute & attrib,
                         Target target = Array,
                         Usage usage = Static)
            : Buffer(attrib, target, usage), buff(buff) {}

        VectorBufferBase(std::vector<element_type> && buff,
                         Target target = Array,
                         Usage usage = Static)
            : Buffer(target, usage), buff(std::move(buff)) {}

        VectorBufferBase(std::vector<element_type> && buff,
                         const Attribute & attrib,
                         Target target = Array,
                         Usage usage = Static)
            : Buffer(attrib, target, usage), buff(std::move(buff)) {}

        VectorBufferBase(VectorBufferBase && other) {
            Buffer(std::move(other));
            buff = std::move(other.buff);
        }

        VectorBufferBase(const VectorBufferBase & other) = delete;
        VectorBufferBase & operator=(const VectorBufferBase & other) = delete;
        VectorBufferBase & operator=(VectorBufferBase && other) = delete;

        /**
         * Load buffer with data from buff.
         *
         * @param buff the data to send to the buffer
         */
        void loadFromPoints(const std::vector<element_type> & buff) {
            this->buff = buff;
            bufferData();
        }

        /**
         * Load buffer with data from buff.
         *
         * @param buff the data to send to the buffer
         */
        void loadFromPoints(std::vector<element_type> && buff) {
            this->buff = std::move(buff);
            bufferData();
        }
    };

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
