#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

// The fixture for testing class Foo.
class GLTest : public ::testing::Test {
protected:
    GLFWwindow * window;

    GLTest();
    ~GLTest() override;
};