#pragma once

#include "Vertex.hpp"
#include "glpp/Buffer.hpp"

namespace glpp::extra {

    /**
     * 2 triangles forming a quad in 2D space.
     */
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
        /**
         * Create a new quad with optional position and size.
         *
         * @param pos the position in 2D space
         * @param size the size
         */
        Quad(const glm::vec2 & pos = glm::vec2(-1),
             const glm::vec2 & size = glm::vec2(2));

        Quad(Quad && other);

        Quad & operator=(Quad && other);

        Quad(const Quad &) = delete;
        Quad & operator=(const Quad &) = delete;

        /**
         * Get the position in 2D space.
         *
         * @return the position
         */
        const glm::vec2 & getPos() const;

        /**
         * Set the position of the quad in 2D space.
         *
         * @param pos the new position
         */
        void setPos(const glm::vec2 & pos);

        /**
         * Get the size of the quad.
         *
         * @return the size
         */
        const glm::vec2 & getSize() const;

        /**
         * Set the size of the quad.
         *
         * @param size the new size
         */
        void setSize(const glm::vec2 & size);

        /**
         * Draw the quad using the currently bound shader.
         */
        void draw() const;
    };
}
