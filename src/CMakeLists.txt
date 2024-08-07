
set(TARGET glpp)

# Find with GLOB if CMake version is at least 3.12, otherwise use set
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp")

set(HEADER_LIST
    extra/Camera.hpp
    extra/debug.hpp
    extra/GeometryBuffer.hpp
    extra/Grid.hpp
    extra/Line.hpp
    extra/Marker.hpp
    extra/Quad.hpp
    extra/Transform.hpp
    extra/Vertex.hpp
    Buffer.hpp
    FrameBuffer.hpp
    Shader.hpp
    Texture.hpp)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/${PROJECT_NAME}/")

set(SOURCE_LIST
    extra/Camera.cpp
    extra/debug.cpp
    extra/GeometryBuffer.cpp
    extra/Grid.cpp
    extra/Line.cpp
    extra/Marker.cpp
    extra/Quad.cpp
    extra/Transform.cpp
    extra/Vertex.cpp
    Buffer.cpp
    FrameBuffer.cpp
    Shader.cpp
    Texture.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/")

add_library(${TARGET} ${SOURCE_LIST} ${HEADER_LIST} ${${PROJECT_NAME}_SOURCE_DIR}/stb/stb_image.h)

target_include_directories(${TARGET} PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/stb>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
    PUBLIC
    OpenGL::OpenGL
    GLEW::GLEW
    glm)

# Check for Inter Procedural Optimization (IPO)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

# Add ALIAS target for use in add_subdirectory or build directory
add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

# IDE support, show headers in a folder
source_group(TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

# Add target to Project Targets for install
install(TARGETS ${TARGET}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) # This is for Windows?

# Include the headers in install
install(DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
