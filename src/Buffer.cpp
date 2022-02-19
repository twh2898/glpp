#include "glpp/Buffer.hpp"

namespace glpp {
    void Attribute::enable() const {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

    void Attribute::disable() const {
        glDisableVertexAttribArray(index);
    }
}

namespace glpp {
    Buffer::Buffer(Target target, Usage usage)
        : target(target), usage(usage), attrib(nullptr) {
        glGenBuffers(1, &buffer);
    }

    Buffer::Buffer(const Attribute & attrib, Target target, Usage usage)
        : Buffer(target, usage) {
        this->attrib = std::make_unique<Attribute>(attrib);
    }

    Buffer::Buffer(Buffer && other) {
        target = other.target;
        usage = other.usage;
        buffer = other.buffer;
        other.buffer = 0;
    }

    Buffer::~Buffer() {
        if (buffer)
            glDeleteBuffers(1, &buffer);
    }

    Buffer::Target Buffer::getTarget() const {
        return target;
    }

    GLuint Buffer::getId() const {
        return buffer;
    }

    Buffer::Usage Buffer::getUsage() const {
        return usage;
    }

    void Buffer::setUsage(Buffer::Usage usage) {
        this->usage = usage;
    }

    Attribute * Buffer::getAttribute() const {
        return attrib.get();
    }

    void Buffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }

    void Buffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Buffer::bufferData(GLsizeiptr size, const void * data) {
        bind();
        glBufferData(target, size, data, usage);
    }

    void Buffer::bufferSubData(GLintptr offset, GLsizeiptr size, const void * data) {
        bind();
        glBufferSubData(target, offset, size, data);
    }
}

namespace glpp {

    BufferArray::BufferArray() : elementBuffer(nullptr) {
        glGenVertexArrays(1, &array);
    }

    BufferArray::BufferArray(std::vector<Buffer> && buffers)
        : buffers(std::move(buffers)) {
        BufferArray();
    }

    BufferArray::BufferArray(const std::vector<Attribute> & attributes) {
        for (auto & attr : attributes) {
            buffers.emplace_back(attr);
        }
        BufferArray();
    }


    BufferArray::~BufferArray() {
        glDeleteVertexArrays(1, &array);
    }

    GLuint BufferArray::getId() const {
        return array;
    }

    std::size_t BufferArray::count() const {
        return buffers.size();
    }

    std::vector<Buffer> & BufferArray::getBuffers() {
        return buffers;
    }

    void BufferArray::bind() const {
        glBindVertexArray(array);
    }

    void BufferArray::unbind() const {
        glBindVertexArray(0);
    }

    void BufferArray::bufferData(size_t index, GLsizeiptr size, const void * data) {
        buffers[index].bufferData(size, data);
        auto * attr = buffers[index].getAttribute();
        if (attr)
            attr->enable();
    }

    void BufferArray::bufferSubData(size_t index,
                                    GLintptr offset,
                                    GLsizeiptr size,
                                    const void * data) {
        buffers[index].bufferSubData(offset, size, data);
    }

    void BufferArray::bufferElements(GLsizeiptr size, const void * data) {
        if (!elementBuffer)
            elementBuffer = std::make_unique<Buffer>(Buffer::Index);
        elementBuffer->bufferData(size, data);
    }

    void BufferArray::drawArrays(GLint first,
                                 GLsizei count,
                                 BufferArray::Mode mode) const {
        bind();
        glDrawArrays(mode, first, count);
    }

    void BufferArray::drawElements(GLsizei count,
                                   GLenum type,
                                   BufferArray::Mode mode,
                                   const void * indices) const {
        bind();
        glDrawElements(mode, count, type, indices);
    }
}

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
