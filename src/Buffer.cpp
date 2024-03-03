#include "glpp/Buffer.hpp"

namespace glpp {
    void Buffer::Attribute::enable() const {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        // Switch starting with OpenGL 4.3
        // https://stackoverflow.com/a/50651756
        if (glVertexBindingDivisor != nullptr)
            glVertexBindingDivisor(index, divisor);
        else
            glVertexAttribDivisor(index, divisor);
        glEnableVertexAttribArray(index);
    }

    void Buffer::Attribute::disable() const {
        glDisableVertexAttribArray(index);
    }

    bool Buffer::Attribute::isInstanced() const {
        return divisor > 0;
    }
}

namespace glpp {
    Buffer::Buffer(Target target) : Buffer({}, target) {}

    Buffer::Buffer(const vector<Attribute> & attrib, Target target)
        : attrib(attrib), target(target) {
        glGenBuffers(1, &buffer);
    }

    Buffer::Buffer(Buffer && other)
        : target(other.target), buffer(other.buffer), attrib(other.attrib) {
        other.buffer = 0;
    }

    Buffer & Buffer::operator=(Buffer && other) {
        target = other.target;
        buffer = other.buffer;
        attrib = other.attrib;
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

    bool Buffer::isInstanced() const {
        for (auto & a : attrib) {
            if (a.isInstanced())
                return true;
        }
        return false;
    }

    void Buffer::attach() const {
        for (auto & a : attrib) {
            a.enable();
        }
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
    using std::make_shared;

    BufferArray::BufferArray() : elementBuffer(nullptr) {
        glGenVertexArrays(1, &array);
    }

    BufferArray::BufferArray(const vector<vector<Buffer::Attribute>> & attributes)
        : BufferArray() {

        bind();
        for (auto & attr : attributes) {
            Buffer::Ptr buff = make_shared<Buffer>(attr, Buffer::Array);
            buffers.push_back(buff);
            buff->bind();
            buff->attach();
        }
    }

    BufferArray::BufferArray(vector<Buffer::Ptr> && buffers) : BufferArray() {

        bind();
        this->buffers = std::move(buffers);
        for (auto & buff : buffers) {
            buff->bind();
            buff->attach();
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

    const vector<Buffer::Ptr> & BufferArray::getBuffers() const {
        return buffers;
    }

    vector<Buffer::Ptr> & BufferArray::getBuffers() {
        return buffers;
    }

    bool BufferArray::isInstanced() const {
        for (auto & b : buffers) {
            if (b->isInstanced())
                return true;
        }
        return false;
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
