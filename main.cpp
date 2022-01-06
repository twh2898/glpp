#include <vector>

#include "Buffer.hpp"
using namespace glpp;

struct Vector {
    float x, y, z;
    float u, v;
    float r, g, b;
}

struct VectorBuffer : public glpp::Buffer {
    std::vector<Vector> buff;

    void * data() override {
        return buff.data();
    }

    size_t size() override {
        return buff.size() * sizeof(Vector);
    }

    size_t count() override {
        return buff.size();
    }
}

int main() {
    Context context;
    Window win(context);

    Buffer b1 {Attribute(0, 3, GL_FLOAT, false, sizeof(float) * 3)};
}