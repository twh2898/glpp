#include <glpp/extra/Buffer.hpp>
using namespace glpp::extra;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    class QuadTest : public GLTest {
    protected:
        glm::vec2 pos;
        glm::vec2 size;

        Quad q;

        QuadTest() : GLTest(), pos(1), size(3), q(pos, size) {}
        ~QuadTest() {}
    };

    TEST_F(QuadTest, Quad) {
        Quad q;
        EXPECT_EQ(glm::vec2(-1), q.getPos());
        EXPECT_EQ(glm::vec2(2), q.getSize());
    }

    TEST_F(QuadTest, Quad_pos_size) {
        EXPECT_EQ(pos, q.getPos());
        EXPECT_EQ(size, q.getSize());
    }

    TEST_F(QuadTest, setPos) {
        q.setPos(glm::vec2(3));
        EXPECT_EQ(glm::vec2(3), q.getPos());
        EXPECT_EQ(size, q.getSize());
    }

    TEST_F(QuadTest, setSize) {
        q.setSize(glm::vec2(4));
        EXPECT_EQ(pos, q.getPos());
        EXPECT_EQ(glm::vec2(4), q.getSize());
    }

    TEST_F(QuadTest, draw) {
        EXPECT_NO_THROW(q.draw());
    }
}