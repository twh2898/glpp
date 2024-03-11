include(${CMAKE_CURRENT_LIST_DIR}/glm.cmake)

if(GLPP_BUILD_EXAMPLES)
    include(${CMAKE_CURRENT_LIST_DIR}/glfw.cmake)
endif()

if(GLPP_BUILD_TESTS)
    include(${CMAKE_CURRENT_LIST_DIR}/gtest.cmake)
endif()
