#pragma once

#include "glpp/Buffer.hpp"
#include "Vertex.hpp"

namespace glpp::extra {

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
}
