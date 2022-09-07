#include <glpp/Texture.hpp>
using namespace glpp;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    class TextureTest : public GLTest {
    protected:
        glm::uvec2 size;
        Texture texture;

        TextureTest() : GLTest(), size(100, 200), texture(size) {}
    };

    TEST_F(TextureTest, Texture_data_size) {
        const unsigned char data[] {0, 0};
        Texture t(data, {1, 2}, 1);
        EXPECT_GT(t.getTextureId(), 0);
        EXPECT_EQ(1, t.getSize().x);
        EXPECT_EQ(2, t.getSize().y);
    }

    /*
    TODO:
    - Wrong nrComponents
    - Mag / Min filter
    - wrap
    - mipmaps
    - samples
    - type
    - internals
    - format
    */

    TEST_F(TextureTest, Texture_size) {
        EXPECT_GT(texture.getTextureId(), 0);
        EXPECT_EQ(size, texture.getSize());
    }

    TEST_F(TextureTest, Move) {
        GLuint id = texture.getTextureId();
        Texture t(std::move(texture));
        EXPECT_EQ(0, texture.getTextureId());
        EXPECT_EQ(id, t.getTextureId());
        EXPECT_EQ(size, t.getSize());
        EXPECT_EQ(texture.getTarget(), t.getTarget());
        EXPECT_EQ(texture.getSamples(), t.getSamples());
    }

    TEST_F(TextureTest, Move_Assign) {
        GLuint id = texture.getTextureId();
        Texture t = std::move(texture);
        EXPECT_EQ(0, texture.getTextureId());
        EXPECT_EQ(id, t.getTextureId());
        EXPECT_EQ(size, t.getSize());
        EXPECT_EQ(texture.getTarget(), t.getTarget());
        EXPECT_EQ(texture.getSamples(), t.getSamples());
    }

    TEST_F(TextureTest, resize) {
        Texture tex({100, 200});
        tex.resize({300, 400});
        EXPECT_EQ(300, tex.getSize().x);
        EXPECT_EQ(400, tex.getSize().y);
    }
}