#include <glpp/extra/Buffer.hpp>
using namespace glpp::extra;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

namespace {
    // The fixture for testing class Foo.
    class VertexTest : public ::testing::Test {
    protected:
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;

        Vertex v;

        VertexTest()
            : pos(1, 2, 3), norm(4, 5, 6), uv(7, 8), v(pos, norm, uv) {}

        void expectNotChanged() const {
            EXPECT_EQ(pos, v.pos);
            EXPECT_EQ(norm, v.norm);
            EXPECT_EQ(uv, v.uv);
        }
    };

    TEST_F(VertexTest, Vertex) {
        Vertex v;
        EXPECT_EQ(glm::vec3(0), v.pos);
        EXPECT_EQ(glm::vec3(0), v.norm);
        EXPECT_EQ(glm::vec2(0), v.uv);
    }

    TEST_F(VertexTest, Vertex_pos_norm_uv) {
        EXPECT_EQ(pos, v.pos);
        EXPECT_EQ(norm, v.norm);
        EXPECT_EQ(uv, v.uv);
    }

    TEST_F(VertexTest, Vertex_Copy) {
        Vertex v2(v);

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Copy_Assign) {
        Vertex v2 = v;

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Vertex_Move) {
        Vertex v2(std::move(v));

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Move_Assign) {
        Vertex v2 = std::move(v);

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Add_Vertex) {
        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        Vertex v3 = v + v2;

        expectNotChanged();

        EXPECT_EQ(glm::vec3(2), v3.pos);
        EXPECT_EQ(glm::vec3(4), v3.norm);
        EXPECT_EQ(glm::vec2(6), v3.uv);
    }

    TEST_F(VertexTest, Add_Vec3) {
        Vertex v2 = v + glm::vec3(1, 2, 3);

        expectNotChanged();

        EXPECT_EQ(glm::vec3(2, 4, 6), v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Subtract) {
        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        Vertex v3 = v - v2;

        expectNotChanged();

        EXPECT_EQ(glm::vec3(0), v3.pos);
        EXPECT_EQ(glm::vec3(0), v3.norm);
        EXPECT_EQ(glm::vec2(0), v3.uv);
    }

    TEST_F(VertexTest, Subtract_Vec3) {
        Vertex v2 = v - glm::vec3(1, 2, 3);

        expectNotChanged();

        EXPECT_EQ(glm::vec3(0), v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST_F(VertexTest, Add_Assign) {
        Vertex v2(v);

        v2 += v;

        expectNotChanged();

        EXPECT_EQ(glm::vec3(2, 4, 6), v2.pos);
        EXPECT_EQ(glm::vec3(8, 10, 12), v2.norm);
        EXPECT_EQ(glm::vec2(14, 16), v2.uv);
    }

    TEST_F(VertexTest, Subtract_Assign) {
        Vertex v2(v);

        v2 -= v;

        expectNotChanged();

        EXPECT_EQ(glm::vec3(0), v2.pos);
        EXPECT_EQ(glm::vec3(0), v2.norm);
        EXPECT_EQ(glm::vec2(0), v2.uv);
    }
}