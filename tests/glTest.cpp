#include "glTest.hpp"

#include <iostream>
#include <stdexcept>
using namespace std;

void error_callback(int error, const char * description) {
    cerr << "Error: " << description << endl;
}

GLTest::GLTest() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    glfwSetErrorCallback(error_callback);

    // Off screen rendering for testing
    // see https://www.glfw.org/docs/latest/context.html#context_offscreen
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 480, "Camera", NULL, NULL);
    if (!window) {
        throw runtime_error("Failed to create GLFW Window");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw runtime_error("Could not initialize GLEW");
    }
}

GLTest::~GLTest() {
    glfwDestroyWindow(window);
    glfwTerminate();
}