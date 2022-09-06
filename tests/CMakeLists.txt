set(TARGET tests)

set(SOURCE_LIST
    test_shader.cpp
    )
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/tests/")

add_executable(${TARGET} ${SOURCE_LIST})
target_link_libraries(${TARGET} PRIVATE glpp::glpp gtest gtest_main glpp)

add_test(NAME ${TARGET} COMMAND ${TARGET})