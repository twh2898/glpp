#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glpp/extra/Camera.hpp>
using namespace glpp::extra;

#include <glfwpp/glfwpp.h>

#include <iostream>
#include <vector>
using namespace std;

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
uniform mat4 mvp;
out vec3 color;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
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
    int width = 640;
    int height = 480;
    glfw::Window window {width, height, "Camera"};

    glfw::makeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        throw runtime_error("Could not initialize GLEW");
    }

    glpp::extra::initDebug();

    Camera camera({width, height}, Camera::Perspective, 83.0f, {0, 0.1, 1});

    window.framebufferSizeEvent.setCallback(
        [](glfw::Window &, int width, int height) {
            glViewport(0, 0, width, height);
        });
    window.cursorPosEvent.setCallback([&](glfw::Window &, double x, double y) {
        const double s = 0.01;
        static double lastX = x;
        static double lastY = y;
        double dx = x - lastX;
        double dy = y - lastY;
        lastX = x;
        lastY = y;
        camera.rotateDolly({dy * s, dx * s, 0});
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
            case glfw::KeyCode::Escape:
                window.setShouldClose(true);
                break;
            default:
                break;
        }
    });

    Shader shader(vertexShaderSource, fragmentShaderSource);
    auto mvp = shader.uniform("mvp");

    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 0.0f, // Bottom Right
        0.0f,  0.5f,  0.0f, // Top Center

        0.0f,  -0.5f, 0.0f, //
        1.0f,  -0.5f, 0.0f, //
        1.0f,  -0.5f, 1.0f, //

        0.5f,  0.0f,  0.0f, //
        0.5f,  1.0f,  0.0f, //
        0.5f,  1.0f,  1.0f, //
    };

    const float colors[] = {
        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center

        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center

        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center
    };

    Attribute a0 {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0};
    Attribute a1 {1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0};

    BufferArray array(vector<vector<Attribute>> {{a0}, {a1}});
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(colors), colors);
    array.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    window.setInputModeCursor(glfw::CursorMode::Disabled);

    while (!window.shouldClose()) {
        glfw::pollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        {
            int w = window.getKey(glfw::KeyCode::W);
            int s = window.getKey(glfw::KeyCode::S);
            int a = window.getKey(glfw::KeyCode::A);
            int d = window.getKey(glfw::KeyCode::D);
            int q = window.getKey(glfw::KeyCode::Q);
            int e = window.getKey(glfw::KeyCode::E);

            if (w || s || a || d || q || e) {
                float dx = (d - a) * 0.1f;
                float dy = (e - q) * 0.1f;
                float dz = (s - w) * 0.1f;
                camera.moveDolly({dx, dy, dz});
            }
        }

        shader.bind();
        mvp.setMat4(camera.projMatrix() * camera.viewMatrix());
        array.drawArrays(Buffer::Triangles, 0, 9);

        window.swapBuffers();
    }

    return 0;
}