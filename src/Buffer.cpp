#include "glpp/Buffer.hpp"

namespace glpp {
    void Attribute::enable() const {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glVertexAttribDivisor(index, divisor);
        glEnableVertexAttribArray(index);
    }

    void Attribute::disable() const {
        glDisableVertexAttribArray(index);
    }
}

namespace glpp {
    Buffer::Buffer(Target target) : target(target) {
        glGenBuffers(1, &buffer);
    }

    Buffer::Buffer(Buffer && other)
        : target(other.target), buffer(other.buffer) {
        other.buffer = 0;
    }

    Buffer & Buffer::operator=(Buffer && other) {
        target = other.target;
        buffer = other.buffer;
        other.buffer = 0;
        return *this;
    }

    Buffer::~Buffer() {
        if (buffer)
            glDeleteBuffers(1, &buffer);
    }

    Buffer::Target Buffer::getTarget() const {
        return target;
    }

    GLuint Buffer::getBufferId() const {
        return buffer;
    }

    void Buffer::bind() const {
        glBindBuffer(target, buffer);
    }

    void Buffer::unbind() const {
        glBindBuffer(target, 0);
    }

    void Buffer::bufferData(GLsizeiptr size, const void * data, Usage usage) {
        bind();
        glBufferData(target, size, data, usage);
    }

    void Buffer::bufferSubData(GLintptr offset, GLsizeiptr size, const void * data) {
        bind();
        glBufferSubData(target, offset, size, data);
    }
}

namespace glpp {
    AttributedBuffer::AttributedBuffer(const std::vector<Attribute> & attrib,
                                       Buffer && buffer)
        : attrib(attrib), buffer(std::move(buffer)) {}

    AttributedBuffer::AttributedBuffer(AttributedBuffer && other)
        : attrib(std::move(other.attrib)), buffer(std::move(other.buffer)) {}

    AttributedBuffer & AttributedBuffer::operator=(AttributedBuffer && other) {
        attrib = std::move(other.attrib);
        buffer = std::move(other.buffer);
        return *this;
    }

    void AttributedBuffer::attach() const {
        buffer.bind();
        for (auto & a : attrib) {
            a.enable();
        }
    }
}

namespace glpp {
    BufferArray::BufferArray() : elementBuffer(nullptr) {
        glGenVertexArrays(1, &array);
    }

    BufferArray::BufferArray(const std::vector<std::vector<Attribute>> & attributes)
        : BufferArray() {

        bind();
        for (auto & attr : attributes) {
            Buffer buffer(Buffer::Array);
            auto & ab = buffers.emplace_back(attr, std::move(buffer));
            ab.attach();
        }
    }

    BufferArray::BufferArray(std::vector<AttributedBuffer> && buffers)
        : BufferArray() {

        bind();
        this->buffers = std::move(buffers);
        for (auto & ab : buffers) {
            ab.buffer.bind();
            ab.attach();
        }
    }

    BufferArray::BufferArray(BufferArray && other)
        : array(other.array),
          buffers(std::move(other.buffers)),
          elementBuffer(std::move(other.elementBuffer)) {

        other.array = 0;
    }

    BufferArray & BufferArray::operator=(BufferArray && other) {
        array = other.array;
        other.array = 0;
        buffers = std::move(other.buffers);
        elementBuffer = std::move(other.elementBuffer);
        return *this;
    }

    BufferArray::~BufferArray() {
        if (array)
            glDeleteVertexArrays(1, &array);
    }

    GLuint BufferArray::getArrayId() const {
        return array;
    }

    std::size_t BufferArray::size() const {
        return buffers.size();
    }

    const std::vector<AttributedBuffer> & BufferArray::getBuffers() const {
        return buffers;
    }

    std::vector<AttributedBuffer> & BufferArray::getBuffers() {
        return buffers;
    }

    void BufferArray::bind() const {
        glBindVertexArray(array);
    }

    void BufferArray::unbind() {
        glBindVertexArray(0);
    }

    void BufferArray::bufferElements(GLsizeiptr size, const void * data, Usage usage) {
        if (!elementBuffer)
            elementBuffer = std::make_unique<Buffer>(Buffer::Index);
        elementBuffer->bufferData(size, data, usage);
    }

    void BufferArray::drawArrays(Mode mode, GLint first, GLsizei count) const {
        bind();
        glDrawArrays(mode, first, count);
    }

    void BufferArray::drawArraysInstanced(Mode mode,
                                          GLint first,
                                          GLsizei count,
                                          GLsizei primcount) const {
        bind();
        glDrawArraysInstanced(mode, first, count, primcount);
    }

    void BufferArray::drawElements(Mode mode,
                                   GLsizei count,
                                   GLenum type,
                                   const void * indices) const {
        bind();
        glDrawElements(mode, count, type, indices);
    }

    void BufferArray::drawElementsInstanced(Mode mode,
                                            GLsizei count,
                                            GLenum type,
                                            const void * indices,
                                            GLsizei primcount) const {
        bind();
        glDrawElementsInstanced(mode, count, type, indices, primcount);
    }
}
