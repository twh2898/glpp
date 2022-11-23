#include "glpp/extra/Quad.hpp"

namespace glpp::extra {
    void Quad::updateBuffer() {
        vertices[0] = vertices[2] = pos.x;
        vertices[4] = vertices[6] = pos.x + size.x;
        vertices[1] = vertices[7] = pos.y + size.y;
        vertices[3] = vertices[5] = pos.y;
    }

    Quad::Quad(const glm::vec2 & pos, const glm::vec2 & size)
        : pos(pos),
          size(size),
          array(std::vector<std::vector<Attribute>> {
              {Attribute {0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}},
              {Attribute {1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}},
          }),
          vertices {0} {
        updateBuffer();
        array.bind();
        array.bufferData(0, sizeof(vertices), vertices);
        array.bufferData(1, sizeof(texCoords), texCoords);
        array.bufferElements(sizeof(indices), indices);
        array.unbind();
    }

    Quad::Quad(Quad && other) : Quad(other.pos, other.size) {}

    Quad & Quad::operator=(Quad && other) {
        pos = other.pos;
        setSize(other.size); // call setSize to update buffer data
        return *this;
    }

    const glm::vec2 & Quad::getPos() const {
        return pos;
    }

    void Quad::setPos(const glm::vec2 & pos) {
        this->pos = pos;
        updateBuffer();
        // bufferSubData calls bind
        array.bufferSubData(0, 0, sizeof(vertices), vertices);
    }

    const glm::vec2 & Quad::getSize() const {
        return size;
    }

    void Quad::setSize(const glm::vec2 & size) {
        this->size = size;
        updateBuffer();
        // bufferSubData calls bind
        array.bufferSubData(0, 0, sizeof(vertices), vertices);
    }

    void Quad::draw() const {
        // drawElements calls bind
        array.drawElements(Buffer::Triangles, 6, GL_UNSIGNED_INT, 0);
    }
}
