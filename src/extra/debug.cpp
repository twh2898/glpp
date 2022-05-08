#include "glpp/extra/debug.hpp"

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <iostream>


namespace glpp::extra {
    static void GLAPIENTRY MessageCallback(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar * message,
                                           const void * userParam) {

        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        std::cerr << "---------------" << std::endl;
        std::cerr << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                std::cerr << "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cerr << "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cerr << "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cerr << "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cerr << "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cerr << "Source: Other";
                break;
        }
        std::cerr << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cerr << "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cerr << "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cerr << "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cerr << "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cerr << "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cerr << "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cerr << "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cerr << "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cerr << "Type: Other";
                break;
        }
        std::cerr << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cerr << "Severity: high";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cerr << "Severity: medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cerr << "Severity: low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cerr << "Severity: notification";
                break;
        }
        std::cerr << std::endl;
        std::cerr << std::endl;
    }

    void initDebug() {
        // During init, enable debug output
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);

        std::cerr << "Debug messages enabled" << std::endl;
    }
}