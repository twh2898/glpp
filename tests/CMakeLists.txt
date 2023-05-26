set(TARGET tests)

macro(define_test name)

    set(SOURCE_LIST
        glTest.cpp
        glTest.hpp
        test_${name}.cpp
    )
    list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

    add_executable(${TARGET}_${name} ${SOURCE_LIST})
    target_link_libraries(${TARGET}_${name} PRIVATE gtest gtest_main glpp glfw)
    
    target_include_directories(${TARGET}_${name} PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}/tests/")
    
    add_test(NAME ${TARGET}_${name} COMMAND ${TARGET}_${name})
endmacro()

define_test(shader)
define_test(uniform)
define_test(texture)
define_test(vertex)
define_test(quad)

define_test(glm_compare)
define_test(extra_Transform)
