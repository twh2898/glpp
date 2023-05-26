#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glpp/extra/Transform.hpp>

// The fixture for testing class Foo.
class GLTest : public ::testing::Test {
protected:
    GLFWwindow * window;

    GLTest();
    ~GLTest() override;
};

inline bool glmFloatEqual(float a, float b, float t = 0.00001) {
    float c = a - b;
    return c <= t && c >= -t;
}

inline bool glmEqual(const glm::quat & a, const glm::quat & b) {
    return glmFloatEqual(a[0], b[0]) && glmFloatEqual(a[1], b[1])
           && glmFloatEqual(a[2], b[2]) && glmFloatEqual(a[3], b[3]);
}

inline bool glmEqual(const glm::vec3 & a, const glm::vec3 & b) {
    return glmFloatEqual(a[0], b[0]) && glmFloatEqual(a[1], b[1])
           && glmFloatEqual(a[2], b[2]);
}

inline bool glmEqual(const glm::mat4 & a, const glm::mat4 & b) {
    return glmEqual(a[0], b[0]) && glmEqual(a[1], b[1]) && glmEqual(a[2], b[2])
           && glmEqual(a[3], b[3]);
}

#define EXPECT_GLM_EQUAL(A, B) EXPECT_TRUE(glmEqual((A), (B)))
#define EXPECT_GLM_NEQUAL(A, B) EXPECT_FALSE(glmEqual((A), (B)))
