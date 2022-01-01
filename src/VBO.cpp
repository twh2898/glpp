#include "glpp/VBO.hpp"

namespace glpp {
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

namespace glpp {
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

namespace glpp {
    BufferArray::BufferArray(std::vector<std::shared_ptr<Buffer>> buffers, BufferArray::Mode mode)
        : buffers(buffers), mode(mode), vao(0), nPoints(0) {

        glGenVertexArrays(1, &vao);
        bind();

        for (auto & buff : buffers) {
            buff->bind();
            buff->enable();
        }

        unbind();
        for (auto & buff : buffers) {
            buff->disable();
            buff->unbind();
        }
    }

    BufferArray::~BufferArray() {
        glDeleteVertexArrays(1, &vao);
    }

    GLuint BufferArray::getVAO() const {
        return vao;
    }

    size_t BufferArray::size() const {
        return nPoints;
    }

    BufferArray::Mode BufferArray::getMode() const {
        return mode;
    }

    void BufferArray::setMode(BufferArray::Mode mode) {
        this->mode = mode;
    }

    void BufferArray::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(mode, 0, nPoints);
        glBindVertexArray(0);
    }

    void BufferArray::bind() const {
        glBindVertexArray(vao);
    }

    void BufferArray::unbind() const {
        glBindVertexArray(0);
    }
}
