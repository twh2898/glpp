# Set up GLFW by fetching it from GitHub

include(FetchContent)

option(GLFW_BUILD_DOCS "Disable" OFF)
option(GLFW_BUILD_EXAMPLES "Disable" OFF)
option(GLFW_BUILD_TESTS "Disable" OFF)

FetchContent_Declare(
    glfw_external
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
    GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(glfw_external)
