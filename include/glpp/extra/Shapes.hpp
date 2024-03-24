#pragma once

#include <memory>
#include <vector>

#include "Vertex.hpp"
#include "glpp/Buffer.hpp"
#include "glpp/Shader.hpp"

namespace glpp::extra {
    using std::shared_ptr;

    /**
     * Simple line with configurable color. The line can be drawn with optional
     * transform. The shader is provided from the Line::shader() static method.
     *
     * Mode is a subset of just line types from Buffer::Mode.
     */
    class Line {
    public:
        using Ptr = shared_ptr<Line>;
        using ConstPtr = const shared_ptr<Line>;

        // Subset of just line types from Buffer::Mode
        enum Mode {
            /// Draw a continuous line between points
            Strip = Buffer::LineStrip,
            /// Use point pairs as separate line segments
            Lines = Buffer::Lines,
            /// Use point pairs as separate line segments
            LineLoop = Buffer::LineLoop,
        };

    private:
        BufferArray::Ptr array;
        int n;
        Mode mode;
        glm::vec4 color;
        std::vector<glm::vec3> points;

        void updateBuffer();

    public:
        /**
         * Create a new Line no segments.
         *
         * @param color the color of line
         * @param mode the point connection mode
         */
        Line(const glm::vec4 & color = glm::vec4(1.0), Mode mode = Strip);

        /**
         * Create a new Line with points.
         *
         * @param points the list of points
         * @param color the color of line
         * @param mode the point connection mode
         */
        Line(const std::vector<glm::vec3> & points,
             const glm::vec4 & color = glm::vec4(1.0),
             Mode mode = Strip);

        /**
         * Create a new Line with points.
         *
         * @param points the list of points
         * @param color the color of line
         * @param mode the point connection mode
         */
        Line(std::vector<glm::vec3> && points,
             const glm::vec4 & color = glm::vec4(1.0),
             Mode mode = Strip);

        /**
         * Create a new Line with two points and color.
         *
         * @param p1 the first point in the line
         * @param p2 the second point in the line
         * @param color the color of line
         * @param mode the point connection mode
         */
        Line(const glm::vec3 & p1,
             const glm::vec3 & p2,
             const glm::vec4 & color = glm::vec4(1.0),
             Mode mode = Strip);

        Line(Line && other);

        Line & operator=(Line && other);

        Line(const Line &) = delete;
        Line & operator=(const Line &) = delete;

        /**
         * Get the number of points in the line.
         *
         * If the mode is set to Lines and the number of points is not even,
         * the last point will be ignored during drawing. This method, however,
         * will still return the size including the last point.
         *
         * @return size_t the number of points
         */
        size_t size() const;

        /**
         * Get a single point at index i.
         *
         * @param i the point index
         * @return a const ref to the point
         *
         * @throws std::out_of_range If i is an invalid index.
         *
         * @see std::vector::at this method is a proxy for reading vector::at
         */
        const glm::vec3 & getPoint(size_t i) const;

        /**
         * Set a single point at index i.
         *
         * @param i the point index
         * @param point the new point value
         *
         * @throws std::out_of_range If i is an invalid index.
         *
         * @see std::vector::at this method is a proxy for writing vector::at
         */
        void setPoint(size_t i, const glm::vec3 & point);

        /**
         * Get the list of points in this Line.
         *
         * @return a const ref to the list of points
         */
        const std::vector<glm::vec3> & getPoints() const;

        /**
         * Set all points to a new list.
         *
         * @param points the new points
         */
        void setPoints(const std::vector<glm::vec3> & points);

        /**
         * Set all points to a new list by move.
         *
         * @param points the new points
         */
        void setPoints(std::vector<glm::vec3> && points);

        /**
         * Get the draw mode for this Line.
         *
         * @return Mode
         */
        Mode getMode() const;

        /**
         * Set the draw mode for this Line.
         *
         * @param mode
         */
        void setMode(Mode mode);

        /**
         * Get the current line color.
         *
         * @return the color
         */
        const glm::vec4 & getColor() const;

        /**
         * Set the line color.
         *
         * This will not affect the x, y or z axis lines.
         *
         * @param color the new color
         */
        void setColor(const glm::vec4 & color);

        /**
         * Draw the line with no transform.
         */
        void draw() const;

        /**
         * Draw the line with the given transform.
         *
         * @param transform the transform for this line
         */
        void draw(const glm::mat4 & transform) const;

        /**
         * Get the line shader.
         *
         * This shader has a mat4 uniform called mvp. This can be set manually
         * or by passing a transform into draw.
         *
         * @return the Shader for lines
         */
        static Shader & shader();
    };
}
