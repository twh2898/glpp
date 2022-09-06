#include <glpp/Shader.hpp>
using namespace glpp;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    class ShaderTest : public GLTest {};

    TEST_F(ShaderTest, shaderCompileException) {
        EXPECT_THROW(
            {
                try {
                    throw ShaderCompileException(1);
                }
                catch (const ShaderCompileException & e) {
                    EXPECT_STREQ("", e.what());
                    throw;
                }
            },
            ShaderCompileException);
    }

    TEST_F(ShaderTest, defaultShader) {
        auto shader = Shader::defaultShader();
        EXPECT_GT(shader.getProgram(), 0);
    }
}