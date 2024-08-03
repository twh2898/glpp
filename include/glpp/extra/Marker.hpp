#pragma once

#include <memory>
#include <vector>

#include "Line.hpp"
#include "Transform.hpp"
#include "Vertex.hpp"
#include "glpp/Buffer.hpp"
#include "glpp/Shader.hpp"

namespace glpp::extra {
    using std::shared_ptr;
    using std::vector;

    class Mark : public Transform {
    public:
        using Ptr = shared_ptr<Mark>;
        using ConstPtr = shared_ptr<Mark>;

    private:
        Line::Ptr line;

    public:
        Mark(const vector<glm::vec3> & points,
             const glm::vec4 & color = glm::vec4(1.0));

        Mark(vector<glm::vec3> && points, const glm::vec4 & color = glm::vec4(1.0));

        Mark(const vector<glm::vec3> & points,
             const glm::vec3 & position,
             const glm::quat & rotation,
             const glm::vec3 & scale,
             const glm::vec4 & color = glm::vec4(1.0));

        Mark(vector<glm::vec3> && points,
             const glm::vec3 & position,
             const glm::quat & rotation,
             const glm::vec3 & scale,
             const glm::vec4 & color = glm::vec4(1.0));

        Mark(const vector<glm::vec3> & points,
             const glm::mat4 & matrix,
             const glm::vec4 & color = glm::vec4(1.0));

        Mark(vector<glm::vec3> && points,
             const glm::mat4 & matrix,
             const glm::vec4 & color = glm::vec4(1.0));

        Mark(const Mark & other) = default;

        Mark(Mark && other) = default;

        Mark & operator=(const Mark & other) = default;

        Mark & operator=(Mark && other) = default;

        virtual ~Mark();

        const glm::vec4 & getColor() const;

        void setColor(const glm::vec4 & color);

        const vector<glm::vec3> & getPoints() const;

        void draw(const glm::mat4 & transform) const;

        void draw() const;
    };

    class AxisMark : public Mark {
    public:
        using Ptr = shared_ptr<AxisMark>;
        using ConstPtr = shared_ptr<AxisMark>;

    public:
        AxisMark(const glm::vec4 & color = glm::vec4(1.0));

        AxisMark(const glm::vec3 & position,
                 const glm::quat & rotation,
                 const glm::vec3 & scale,
                 const glm::vec4 & color = glm::vec4(1.0));

        AxisMark(const glm::mat4 & matrix,
                 const glm::vec4 & color = glm::vec4(1.0));

        AxisMark(const AxisMark & other) = default;

        AxisMark(AxisMark && other) = default;

        AxisMark & operator=(const AxisMark & other) = default;

        AxisMark & operator=(AxisMark && other) = default;

        virtual ~AxisMark();
    };

    class DiamondMark : public Mark {
    public:
        using Ptr = shared_ptr<DiamondMark>;
        using ConstPtr = shared_ptr<DiamondMark>;

    public:
        DiamondMark(const glm::vec4 & color = glm::vec4(1.0));

        DiamondMark(const glm::vec3 & position,
                    const glm::quat & rotation,
                    const glm::vec3 & scale,
                    const glm::vec4 & color = glm::vec4(1.0));

        DiamondMark(const glm::mat4 & matrix,
                    const glm::vec4 & color = glm::vec4(1.0));

        DiamondMark(const DiamondMark & other) = default;

        DiamondMark(DiamondMark && other) = default;

        DiamondMark & operator=(const DiamondMark & other) = default;

        DiamondMark & operator=(DiamondMark && other) = default;

        virtual ~DiamondMark();
    };
}
