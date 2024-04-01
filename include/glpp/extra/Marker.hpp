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
        bool visible;
        glm::vec4 color;

    protected:
        virtual const vector<glm::vec3> & shapePoints() const = 0;

    public:
        using Transform::Transform;

        virtual ~Mark();

        const glm::vec4 & getColor() const;

        void setColor(const glm::vec4 & color);

        bool isVisible() const;

        void show();

        void hide();

        vector<glm::vec3> getPoints() const;
    };

    class AxisMark : public Mark {
    public:
        using Ptr = shared_ptr<AxisMark>;
        using ConstPtr = shared_ptr<AxisMark>;

    protected:
        const vector<glm::vec3> & shapePoints() const override;

    public:
        using Mark::Mark;

        virtual ~AxisMark();
    };

    class DiamondMark : public Mark {
    public:
        using Ptr = shared_ptr<DiamondMark>;
        using ConstPtr = shared_ptr<DiamondMark>;

    protected:
        const vector<glm::vec3> & shapePoints() const override;

    public:
        using Mark::Mark;

        virtual ~DiamondMark();
    };

    // TODO: Marker base class s.t. Markers can have different shapes
    // eg. Cross, Normal, 3d Diamond, Bounding Box

    class LiveMarker {
    public:
        using Ptr = shared_ptr<LiveMarker>;
        using ConstPtr = const shared_ptr<LiveMarker>;

    private:
        Line::Ptr line;
        mutable vector<glm::vec3> marks;
        float size;

        void updateLine() const;

    public:
        LiveMarker(float size = 0.1, const glm::vec4 & color = glm::vec4(1.0));

        virtual ~LiveMarker();

        // TODO: other constructors

        const float & getSize() const;

        void setSize(const float & size);

        void addMark(const glm::vec3 & pos);

        void draw() const;
    };

    class Markers {
    public:
        using Ptr = shared_ptr<Markers>;
        using ConstPtr = shared_ptr<Markers>;

    private:
        Line::Ptr line;
        vector<Mark::Ptr> marks;

        void updateLine();

    public:
        Markers();

        virtual ~Markers();

        void addMark(const Mark::Ptr & mark);

        /**
         * Draw the markers with no transform.
         */
        void draw() const;

        /**
         * Draw the markers with the given transform.
         *
         * @param transform the transform for these markers
         */
        void draw(const glm::mat4 & transform) const;

        /**
         * Get the markers shader.
         *
         * This shader has a mat4 uniform called mvp. This can be set manually
         * or by passing a transform into draw.
         *
         * @return the Shader for markers
         */
        static Shader & shader();
    };
}
