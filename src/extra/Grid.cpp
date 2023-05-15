#include "glpp/extra/Grid.hpp"

#include <vector>

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
uniform mat4 vp;
uniform mat4 model;
out vec4 color;
void main() {
    gl_Position = vp * model * vec4(aPos, 1.0);
    color = aCol;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec4 color;
out vec4 FragColor;
void main() {
    FragColor = color;
})";

namespace glpp::extra {
    using std::vector;

    void Grid::updateBuffer() {
        vector<glm::vec3> vertices;
        vector<glm::vec4> colors;

        for (int i = 0; i <= size; i++) {
            // TODO: color axis

            // X axis
            vertices.emplace_back(size, 0, i);
            if (i == 0 && colorAxis)
                colors.push_back({1, 0, 0, color.a});
            else
                colors.push_back(color);

            vertices.emplace_back(-size, 0, i);
            if (i == 0 && colorAxis)
                colors.push_back({1, 0, 0, color.a});
            else
                colors.push_back(color);

            if (i > 0) {
                vertices.emplace_back(size, 0, -i);
                colors.push_back(color);

                vertices.emplace_back(-size, 0, -i);
                colors.push_back(color);
            }

            // Z axis
            vertices.emplace_back(i, 0, size);
            if (i == 0 && colorAxis)
                colors.push_back({0, 0, 1, color.a});
            else
                colors.push_back(color);

            vertices.emplace_back(i, 0, -size);
            if (i == 0 && colorAxis)
                colors.push_back({0, 0, 1, color.a});
            else
                colors.push_back(color);

            if (i > 0) {
                vertices.emplace_back(-i, 0, size);
                colors.push_back(color);

                vertices.emplace_back(-i, 0, -size);
                colors.push_back(color);
            }
        }

        n = vertices.size();

        array.bind();
        array.bufferData(0, n * sizeof(glm::vec3), vertices.data());
        array.bufferData(1, n * sizeof(glm::vec4), colors.data());
        array.unbind();
    }

    Grid::Grid(int size, const glm::vec4 & color, bool colorAxis)
        : n(0),
          size(size),
          color(color),
          colorAxis(colorAxis),
          array(std::vector<std::vector<Attribute>> {
              {Attribute {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0}},
              {Attribute {1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0}},
          }) {
        updateBuffer();
    }

    Grid::Grid(Grid && other)
        : Grid(other.size, other.color, other.colorAxis) {}

    Grid & Grid::operator=(Grid && other) {
        size = other.size;
        color = other.color;
        colorAxis = other.colorAxis;
        updateBuffer();
        return *this;
    }

    int Grid::getSize() const {
        return size;
    }

    void Grid::setSize(int size) {
        this->size = size;
        updateBuffer();
    }

    const glm::vec4 & Grid::getColor() const {
        return color;
    }

    void Grid::setColor(const glm::vec4 & color) {
        this->color = color;
        updateBuffer();
    }

    void Grid::draw() const {
        // drawArrays calls bind
        array.drawArrays(Buffer::Lines, 0, n);
    }

    void Grid::draw(const glm::mat4 & transform) const {
        shader().bind();
        shader().uniform("vp").setMat4(transform);
        shader().uniform("model").setMat4(glm::mat4(1));
        array.drawArrays(Buffer::Lines, 0, n);
    }

    Shader & Grid::shader() {
        static Shader shader(vertexShaderSource, fragmentShaderSource);
        return shader;
    }
}
