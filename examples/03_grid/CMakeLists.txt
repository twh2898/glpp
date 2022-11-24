
set(TARGET 03_grid)

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET} glpp::glpp glfw)
