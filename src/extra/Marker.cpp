#include "glpp/extra/Marker.hpp"

namespace glpp::extra {
    using std::make_shared;

    Mark::Mark()
        : Transform(),
          visible(true),
          line(make_shared<Line>(glm::vec4(1.0), Line::Lines)),
          firstDraw(true) {}

    Mark::Mark(const glm::vec3 & position,
               const glm::quat & rotation,
               const glm::vec3 & scale)
        : Transform(position, rotation, scale),
          visible(true),
          line(make_shared<Line>(glm::vec4(1.0), Line::Lines)),
          firstDraw(true) {}

    Mark::Mark(const glm::mat4 & matrix)
        : Transform(matrix),
          visible(true),
          line(make_shared<Line>(glm::vec4(1.0), Line::Lines)),
          firstDraw(true) {}

    Mark::~Mark() {
        Transform::~Transform();
    }

    const glm::vec4 & Mark::getColor() const {
        return line->getColor();
    }

    void Mark::setColor(const glm::vec4 & color) {
        line->setColor(color);
    }

    bool Mark::isVisible() const {
        return visible;
    }

    void Mark::show() {
        visible = true;
    }

    void Mark::hide() {
        visible = false;
    }

    vector<glm::vec3> Mark::getPoints() const {
        return shapePoints();
    }

    void Mark::draw() const {
        if (!visible)
            return;

        if (firstDraw) {
            // NOTE this is here instead of constructor because constructor
            // can't call virtual method
            line->setPoints(shapePoints());
            firstDraw = false;
        }

        line->draw();
    }

    void Mark::draw(const glm::mat4 & transform) const {
        if (!visible)
            return;

        if (firstDraw) {
            // NOTE this is here instead of constructor because constructor
            // can't call virtual method
            line->setPoints(shapePoints());
            firstDraw = false;
        }

        line->draw(transform * toMatrix());
    }
}

namespace glpp::extra {
    const vector<glm::vec3> & AxisMark::shapePoints() const {
        static vector<glm::vec3> points {
            {-1, 0, 0}, {1, 0, 0}, // X
            {0, -1, 0}, {0, 1, 0}, // Y
            {0, 0, -1}, {0, 0, 1}, // Z
        };

        return points;
    }

    AxisMark::~AxisMark() {
        Mark::~Mark();
    }
}

namespace glpp::extra {
    const vector<glm::vec3> & DiamondMark::shapePoints() const {
        static vector<glm::vec3> points {
            {-1, 0, 0}, {0, 0, -1}, //
            {0, 0, -1}, {1, 0, 0}, //
            {1, 0, 0},  {0, 0, 1}, //
            {0, 0, 1},  {-1, 0, 0}, //

            {0, 1, 0},  {0, 0, -1}, //
            {0, 1, 0},  {1, 0, 0}, //
            {0, 1, 0},  {0, 0, 1}, //
            {0, 1, 0},  {-1, 0, 0}, //

            {0, -1, 0}, {0, 0, -1}, //
            {0, -1, 0}, {1, 0, 0}, //
            {0, -1, 0}, {0, 0, 1}, //
            {0, -1, 0}, {-1, 0, 0}, //
        };

        return points;
    }

    DiamondMark::~DiamondMark() {
        Mark::~Mark();
    }
}

namespace glpp::extra {
    using std::make_shared;

    void LiveMarker::updateLine() const {
        vector<glm::vec3> points;

        for (auto & pos : marks) {
            float x = pos.x;
            float y = pos.y;
            float z = pos.z;
            float s = size / 2;

            points.emplace_back(x - s, y, z);
            points.emplace_back(x + s, y, z);

            points.emplace_back(x, y - s, z);
            points.emplace_back(x, y + s, z);

            points.emplace_back(x, y, z - s);
            points.emplace_back(x, y, z + s);
        }

        line->setPoints(std::move(points));
    }

    LiveMarker::LiveMarker(float size, const glm::vec4 & color)
        : line(make_shared<Line>(color)), size(size) {}

    LiveMarker::~LiveMarker() {}

    const float & LiveMarker::getSize() const {
        return size;
    }

    void LiveMarker::setSize(const float & size) {
        this->size = size;
    }

    void LiveMarker::addMark(const glm::vec3 & pos) {
        marks.push_back(pos);
    }

    void LiveMarker::draw() const {
        updateLine();
        line->draw(glm::mat4(1.0));
        marks.clear();
    }
}

namespace glpp::extra {
    void Markers::updateLine() {
        vector<glm::vec3> points;

        for (auto & mark : marks) {
            auto markPoints = mark->getPoints();
            points.insert(points.end(), markPoints.begin(), markPoints.end());
        }

        line->setPoints(std::move(points));
    }

    Markers::Markers() {}

    Markers::~Markers() {}

    void Markers::addMark(const Mark::Ptr & mark) {
        marks.push_back(mark);
    }

    void Markers::draw() const {
        line->draw();
    }

    void Markers::draw(const glm::mat4 & transform) const {
        line->draw(transform);
    }

    Shader & Markers::shader() {
        return Line::shader();
    }
}
