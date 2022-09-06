#include "glpp/extra/Buffer.hpp"

namespace glpp::extra {
    Vertex::Vertex() : pos(0), norm(0), uv(0) {}

    Vertex::Vertex(const glm::vec3 & pos, const glm::vec3 & norm, const glm::vec2 & uv)
        : pos(pos), norm(norm), uv(uv) {}

    Vertex Vertex::operator+(const Vertex & other) const {
        return Vertex(pos + other.pos, norm + other.norm, uv + other.uv);
    }

    Vertex Vertex::operator+(const glm::vec3 & offset) const {
        return Vertex(pos + offset, norm, uv);
    }

    Vertex Vertex::operator-(const Vertex & other) const {
        return Vertex(pos - other.pos, norm - other.norm, uv - other.uv);
    }

    Vertex Vertex::operator-(const glm::vec3 & offset) const {
        return Vertex(pos - offset, norm, uv);
    }

    Vertex & Vertex::operator+=(const Vertex & other) {
        pos += other.pos;
        norm += other.norm;
        uv += other.uv;
        return *this;
    }

    Vertex & Vertex::operator-=(const Vertex & other) {
        pos -= other.pos;
        norm -= other.norm;
        uv -= other.uv;
        return *this;
    }
}

namespace glpp::extra {
    void Quad::updateBuffer() {
        vertices[0] = vertices[2] = x;
        vertices[4] = vertices[6] = x + width;
        vertices[1] = vertices[7] = y + height;
        vertices[3] = vertices[5] = y;
    }

    Quad::Quad(float x, float y, float w, float h)
        : array(std::vector<std::vector<Attribute>> {
            {Attribute {0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}},
            {Attribute {1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0}},
        }),
          vertices {0},
          x(x),
          y(y),
          width(w),
          height(h) {
        updateBuffer();
        array.bind();
        array.bufferData(0, sizeof(vertices), vertices);
        array.bufferData(1, sizeof(texCoords), texCoords);
        array.bufferElements(sizeof(indices), indices);
        array.unbind();
    }

    void Quad::setPos(float x, float y) {
        this->x = x;
        this->y = y;
        updateBuffer();
        array.bufferSubData(0, 0, sizeof(vertices), vertices);
    }

    void Quad::setSize(float w, float h) {
        width = w;
        height = h;
        updateBuffer();
        array.bufferSubData(0, 0, sizeof(vertices), vertices);
    }

    void Quad::draw() const {
        array.drawElements(Buffer::Triangles, 6, GL_UNSIGNED_INT, 0);
    }
}

namespace glpp::extra {
    void draw_array(const std::vector<Vertex> & vertices, GLenum mode) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &vertices[0].pos.x);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &vertices[0].norm.x);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &vertices[0].uv.x);

        glDrawArrays(mode, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void draw_quad(const glm::vec2 & pos, const glm::vec2 & size) {
        std::vector<Vertex> vertices {
            {{pos.x, pos.y + size.y, 0.0}, {0, 0, 1}, {0, 1}},
            {{pos.x, pos.y, 0.0}, {0, 0, 1}, {0, 0}},
            {{pos.x + size.x, pos.y, 0.0}, {0, 0, 1}, {1, 0}},

            {{pos.x, pos.y + size.y, 0.0}, {0, 0, 1}, {0, 1}},
            {{pos.x + size.x, pos.y, 0.0}, {0, 0, 1}, {1, 0}},
            {{pos.x + size.x, pos.y + size.y, 0.0}, {0, 0, 1}, {1, 1}},
        };

        draw_array(vertices, GL_TRIANGLES);
    }
}
