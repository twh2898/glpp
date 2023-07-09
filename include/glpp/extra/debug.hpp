#pragma once

namespace glpp::extra {
    /**
     * Enable GL_DEBUG_OUTPUT and setup glDebugMessageCallback.
     *
     * This function will write a message to stderr noting that debug messages
     * are enabled.
     */
    void initDebug();
}
