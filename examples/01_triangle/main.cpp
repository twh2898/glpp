#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glfwpp/glfwpp.h>

#include <iostream>
#include <vector>
using namespace std;

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 color;
void main() {
    gl_Position = vec4(aPos, 1.0);
    color = aCol;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;
void main() {
    FragColor = vec4(color, 1.0);
})";

int main() {
    auto GLFW = glfw::init();
    glfw::WindowHints {.contextVersionMajor = 3,
                       .contextVersionMinor = 2,
                       .openglProfile = glfw::OpenGlProfile::Core}
        .apply();
    glfw::Window window {640, 480, "Hello GLFWPP"};

    glfw::makeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        throw runtime_error("Could not initialize GLEW");
    }

    glpp::extra::initDebug();

    window.framebufferSizeEvent.setCallback(
        [](glfw::Window &, int width, int height) {
            glViewport(0, 0, width, height);
        });
    window.keyEvent.setCallback([&](glfw::Window &, glfw::KeyCode keyCode_,
                                    int scanCode_, glfw::KeyState state_,
                                    glfw::ModifierKeyBit modifiers_) {
        float val = 0.5;
        if (modifiers_ & glfw::ModifierKeyBit::Control) {
            val += 0.25;
        }
        if (modifiers_ & glfw::ModifierKeyBit::Shift) {
            val -= 0.25;
        }
        switch (keyCode_) {
            case glfw::KeyCode::R:
                glClearColor(val, 0.0, 0.0, val);
                break;
            case glfw::KeyCode::G:
                glClearColor(0.0, val, 0.0, val);
                break;
            case glfw::KeyCode::B:
                glClearColor(0.0, 0.0, val, val);
                break;
            case glfw::KeyCode::M:
                glClearColor(val, 0.0, val, val);
                break;
            case glfw::KeyCode::C:
                glClearColor(0.0, val, val, val);
                break;
            case glfw::KeyCode::Y:
                glClearColor(val, val, 0.0, val);
                break;
            case glfw::KeyCode::K:
                glClearColor(0.0, 0.0, 0.0, val);
                break;
            case glfw::KeyCode::Escape:
                window.setShouldClose(true);
                break;
            default:
                break;
        }
    });

    Shader shader(vertexShaderSource, fragmentShaderSource);

    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 0.0f, // Bottom Right
        0.0f,  0.5f,  0.0f // Top Center
    };

    const float colors[] = {
        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0 // Top Center
    };

    const unsigned int indices[] = {
        0, 1, 2, // First Triangle
    };

    Attribute a0 {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0};
    Attribute a1 {1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0};

    BufferArray array(vector<vector<Attribute>> {{a0}, {a1}});
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(colors), colors);
    array.bufferElements(sizeof(indices), indices);
    array.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window.shouldClose()) {
        glfw::pollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        array.drawArrays(Buffer::Triangles, 0, 3);
        array.drawElements(Buffer::Triangles, 3, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
    }

    return 0;
}