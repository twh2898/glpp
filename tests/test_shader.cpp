#include <glpp/Shader.hpp>
using namespace glpp;

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <stdexcept>
using namespace std;

namespace {

    // The fixture for testing class Foo.
    class ShaderTest : public ::testing::Test {
    protected:
        GLFWwindow * window;

        ShaderTest() {
            if (!glfwInit()) {
                throw runtime_error("Failed to initialize GLFW");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            int width = 640;
            int height = 480;
            window = glfwCreateWindow(width, height, "Camera", NULL, NULL);
            if (!window) {
                throw runtime_error("Failed to create GLFW Window");
            }

            glfwMakeContextCurrent(window);

            if (glewInit() != GLEW_OK) {
                throw runtime_error("Could not initialize GLEW");
            }
        }

        ~ShaderTest() override {
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Class members declared here can be used by all tests in the test
        // suite for Foo.
    };

    TEST(ShaderTest, defaultShader) {
        auto shader = Shader::defaultShader();
        EXPECT_GT(shader.getProgram(), 0);
    }
}