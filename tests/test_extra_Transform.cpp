#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glpp/extra/Transform.hpp>
#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    using glpp::extra::Transform;
    using glm::vec3;
    using glm::quat;
    using glm::mat4;

    /**
     * Empty constructor should have default position and rotation of 0 with
     * default scale of 1.
     */
    TEST(TransformTest, EmptyConstructor) {
        Transform t;
        EXPECT_GLM_EQUAL(t.getPosition(), vec3(0, 0, 0));
        EXPECT_GLM_EQUAL(t.getRotationEuler(), vec3(0, 0, 0));
        EXPECT_GLM_EQUAL(t.getScale(), vec3(1, 1, 1));
    }

    /**
     * Component constructor should not modify components.
     */
    TEST(TransformTest, ComponentsConstructor) {
        vec3 pos(1, 2, 3);
        quat rot(vec3(4, 5, 6));
        vec3 scale(7, 8, 9);
        Transform t(pos, rot, scale);
        EXPECT_GLM_EQUAL(t.getPosition(), pos);
        EXPECT_GLM_EQUAL(t.getRotation(), rot);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
    }

    static void printQuat(const quat & q) {
        std::cout << q.w << " " << q.x << " " << q.y << " " << q.z << std::endl;
    }

    /**
     * Matrix constructor should decompose into components without further
     * modification.
     */
    TEST(TransformTest, MatrixConstructor) {
        vec3 pos(1, 2, 3);
        quat rot(vec3(4, 5, 6));
        vec3 scale(7, 8, 9);
        auto mTranslate = glm::translate(glm::mat4(1), pos);
        auto mRotate = glm::toMat4(rot);
        auto mScale = glm::scale(glm::mat4(1), scale);
        mat4 matrix = mTranslate * mRotate * mScale;

        Transform t(matrix);
        EXPECT_GLM_EQUAL(t.getPosition(), pos);
        EXPECT_GLM_EQUAL(t.getRotation(), rot);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
        EXPECT_GLM_EQUAL(t.toMatrix(), matrix);
    }

    /**
     * Matrix constructor should decompose into components without further
     * modification. Calculated matrix should match the input matrix.
     */
    TEST(TransformTest, MatrixConstructor2) {
        vec3 pos(1, 2, 3);
        quat rot(vec3(4, 5, 6));
        vec3 scale(7, 8, 9);
        Transform t1(pos, rot, scale);
        Transform t2(t1.toMatrix());

        EXPECT_GLM_EQUAL(t2.getPosition(), pos);
        EXPECT_GLM_EQUAL(t2.getRotation(), rot);
        EXPECT_GLM_EQUAL(t2.getScale(), scale);
        EXPECT_GLM_EQUAL(t2.toMatrix(), t1.toMatrix());
    }

    // TODO: test everything else
}
