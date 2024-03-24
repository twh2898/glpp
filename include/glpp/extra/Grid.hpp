#pragma once

#include <memory>

#include "Vertex.hpp"
#include "glpp/Buffer.hpp"
#include "glpp/Shader.hpp"

namespace glpp::extra {
    using std::shared_ptr;

    /**
     * Simple grid with configurable size and color. The grid can be drawn with
     * optional transform. The shader is provided from the Grid::shader() static
     * method.
     */
    class Grid {
    public:
        using Ptr = shared_ptr<Grid>;
        using ConstPtr = const shared_ptr<Grid>;

    private:
        BufferArray::Ptr array;
        int n;
        int size;
        glm::vec4 color;
        bool colorAxis;

        void updateBuffer();

    public:
        /**
         * Create a new Grid with size, color and optional flag to color the x,
         * y and z axis lines with different colors.
         *
         * If colorAxis is false, the axis will also use the color parameter.
         *
         * @param size the number of cells from the center
         * @param color the color of grid lines
         * @param colorAxis use a unique color for x, y and z axis lines
         */
        Grid(int size, const glm::vec4 & color, bool colorAxis = false);

        Grid(Grid && other);

        Grid & operator=(Grid && other);

        Grid(const Grid &) = delete;
        Grid & operator=(const Grid &) = delete;

        /**
         * Get the current grid size in number of cells from the center.
         *
         * @return the number of cells
         */
        int getSize() const;

        /**
         * Set the grid size in number of cells from the center on the x and z
         * axis.
         *
         * The grid size will be double this, because the size represents a
         * "radius" measured from the center to the edge.
         *
         * @param size number of cells from the center to the edge in x and z
         */
        void setSize(int size);

        /**
         * Get the current grid color.
         *
         * @return the color
         */
        const glm::vec4 & getColor() const;

        /**
         * Set the grid color.
         *
         * This will not affect the x, y or z axis lines.
         *
         * @param color the new color
         */
        void setColor(const glm::vec4 & color);

        /**
         * Draw the grid with no transform.
         */
        void draw() const;

        /**
         * Draw the grid with the given transform.
         *
         * @param transform the transform for this grid
         */
        void draw(const glm::mat4 & transform) const;

        /**
         * Get the grid shader.
         *
         * This shader has a mat4 uniform called mvp. This can be set manually
         * or by passing a transform into draw.
         *
         * @return the Shader for a grid
         */
        static Shader & shader();
    };
}
