#pragma once

#include <vector>

#include "glpp/Buffer.hpp"

namespace glpp::extra {
    using std::vector;

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
         * Subtract offset from just the position field.
         *
         * @param offset the offset to add to pos
         *
         * @return a new Vertex with the difference of pos and offset
         */
        Vertex operator-(const glm::vec3 & offset) const;

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

    class VertexBufferArray : public BufferArray {
    private:
        using BufferArray::bufferData;
        using BufferArray::bufferSubData;

    public:
        VertexBufferArray();

        VertexBufferArray(VertexBufferArray && other);

        VertexBufferArray & operator=(VertexBufferArray && other);

        VertexBufferArray(const VertexBufferArray &) = delete;
        VertexBufferArray & operator=(const VertexBufferArray &) = delete;

        ~VertexBufferArray();

        inline void bufferData(const vector<Vertex> & data,
                               Usage usage = Usage::Static) {
            bufferData(data.size(), data.data(), usage);
        }

        void bufferData(GLsizeiptr size,
                        const Vertex * data,
                        Usage usage = Usage::Static);

        void bufferSubData(GLintptr offset, GLsizeiptr size, const Vertex * data);
    };
}
