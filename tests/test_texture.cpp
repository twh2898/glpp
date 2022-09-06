#include <glpp/Texture.hpp>
using namespace glpp;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    class TextureTest : public GLTest {};

    TEST_F(TextureTest, TextureImage) {
        const unsigned char data[] {0, 0};
        Texture tex(data, {1, 2}, 1);
        EXPECT_GT(tex.getTextureId(), 0);
        EXPECT_EQ(1, tex.getSize().x);
        EXPECT_EQ(2, tex.getSize().y);
    }

    TEST_F(TextureTest, TextureSize) {
        Texture tex({100, 200});
        EXPECT_GT(tex.getTextureId(), 0);
        EXPECT_EQ(100, tex.getSize().x);
        EXPECT_EQ(200, tex.getSize().y);
    }

    TEST_F(TextureTest, resize) {
        Texture tex({100, 200});
        tex.resize({300, 400});
        EXPECT_EQ(300, tex.getSize().x);
        EXPECT_EQ(400, tex.getSize().y);
    }
}