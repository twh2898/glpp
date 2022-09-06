#pragma once

#include "glpp/Buffer.hpp"

namespace glpp::extra {

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


    class Quad {
        BufferArray array;
        float vertices[8];
        glm::vec2 pos;
        glm::vec2 size;

        const float texCoords[8] = {
            0.0f, 1.0f, //
            0.0f, 0.0f, //
            1.0f, 0.0f, //
            1.0f, 1.0f, //
        };

        const unsigned int indices[6] = {
            0, 1, 2, //
            0, 2, 3, //
        };

        void updateBuffer();

    public:
        Quad(const glm::vec2 & pos = glm::vec2(-1),
             const glm::vec2 & size = glm::vec2(2));

        Quad(Quad && other);
        
        Quad & operator=(Quad && other);
        
        Quad(const Quad &) = delete;
        Quad & operator=(const Quad &) = delete;

        const glm::vec2 & getPos() const;

        void setPos(const glm::vec2 & pos);

        const glm::vec2 & getSize() const;

        void setSize(const glm::vec2 & size);

        void draw() const;
    };

    void draw_array(const std::vector<Vertex> & vertices, GLenum mode);

    void draw_quad(const glm::vec2 & pos, const glm::vec2 & size);
}
