#include <glpp/Shader.hpp>
using namespace glpp;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

static const char * vertexShaderSource = R"(
#version 330 core
uniform float v;
void main() {
})";

static const char * fragmentShaderSource = R"(
#version 330 core
void main() {
})";

namespace {
    class ShaderTest : public GLTest {
    protected:
        Shader shader;
        Uniform mvp, gTexture;

        ShaderTest()
            : GLTest(),
              shader(Shader::defaultShader()),
              mvp(shader.uniform("mvp")),
              gTexture(shader.uniform("gTexture")) {}
    };

    TEST_F(ShaderTest, shaderCompileException) {
        EXPECT_THROW(
            {
                try {
                    Shader("", "");
                }
                catch (const ShaderCompileException & e) {
                    EXPECT_STREQ(
                        "0:1(1): error: syntax error, unexpected end of file\n",
                        e.what());
                    throw;
                }
            },
            ShaderCompileException);
        EXPECT_THROW(
            {
                try {
                    Shader::fromFragmentSource("");
                }
                catch (const ShaderCompileException & e) {
                    EXPECT_STREQ(
                        "0:1(1): error: syntax error, unexpected end of file\n",
                        e.what());
                    throw;
                }
            },
            ShaderCompileException);
        EXPECT_THROW(
            {
                try {
                    Shader::fromFragmentSource(
                        "#version 330\nvoid main(){broken();}");
                }
                catch (const ShaderCompileException & e) {
                    EXPECT_STREQ("0:2(13): error: no function with name 'broken'\n",
                                 e.what());
                    throw;
                }
            },
            ShaderCompileException);
    }

    // TODO: I don't know how to cause a link error
    // TEST_F(ShaderTest, shaderLinkException) {
    //     EXPECT_THROW(
    //         {
    //             try {
    //                 Shader::fromFragmentSource("#version 330\nvoid main()
    //                 {broken();}");
    //             }
    //             catch (const ShaderLinkException & e) {
    //                 EXPECT_STREQ("0:2(14): error: no function with name
    //                 'broken'\n", e.what()); throw;
    //             }
    //         },
    //         ShaderLinkException);
    // }

    TEST_F(ShaderTest, Shader) {
        Shader s(vertexShaderSource, fragmentShaderSource);
        EXPECT_GT(s.getProgram(), 0);
    }

    TEST_F(ShaderTest, Move) {
        GLuint p = shader.getProgram();
        Shader s2(std::move(shader));
        EXPECT_EQ(0, shader.getProgram());
        EXPECT_EQ(p, s2.getProgram());
    }

    TEST_F(ShaderTest, Move_Assign) {
        GLuint p = shader.getProgram();
        Shader s2 = std::move(shader);
        EXPECT_EQ(0, shader.getProgram());
        EXPECT_EQ(p, s2.getProgram());
    }

    TEST_F(ShaderTest, getUniform) {
        EXPECT_EQ(0, mvp.getLocation());
        EXPECT_EQ(1, gTexture.getLocation());
    }

    TEST_F(ShaderTest, getBadUniform) {
        Uniform u = shader.uniform("bad");
        EXPECT_EQ(-1, u.getLocation());
    }

    TEST_F(ShaderTest, bind) {
        EXPECT_NO_THROW(shader.bind());
    }

    TEST_F(ShaderTest, unbind) {
        EXPECT_NO_THROW(shader.unbind());
    }

    TEST_F(ShaderTest, defaultShader) {
        EXPECT_GT(shader.getProgram(), 0);
    }

    TEST_F(ShaderTest, fromFragmentSource) {
        Shader s = Shader::fromFragmentSource(fragmentShaderSource);
        EXPECT_GT(s.getProgram(), 0);
    }
}