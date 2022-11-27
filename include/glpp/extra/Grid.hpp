#pragma once

#include "Vertex.hpp"
#include "glpp/Buffer.hpp"
#include "glpp/Shader.hpp"

namespace glpp::extra {

    class Grid {
        BufferArray array;
        int n;
        int size;
        glm::vec4 color;
        bool colorAxis;

        void updateBuffer();

    public:
        Grid(int size, const glm::vec4 & color, bool colorAxis = false);

        Grid(Grid && other);

        Grid & operator=(Grid && other);

        Grid(const Grid &) = delete;
        Grid & operator=(const Grid &) = delete;

        int getSize() const;

        void setSize(int size);

        const glm::vec4 & getColor() const;

        void setColor(const glm::vec4 & color);

        void draw() const;

        static Shader shader();
    };
}
