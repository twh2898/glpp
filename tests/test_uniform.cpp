#include <glpp/Shader.hpp>
using namespace glpp;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    TEST(UniformTest, Uniform) {
        Uniform u;
        EXPECT_EQ(0, u.getLocation());
    }

    TEST(UniformTest, Uniform_location) {
        Uniform u(1);
        EXPECT_EQ(1, u.getLocation());
    }
}