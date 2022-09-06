#include <glpp/extra/Buffer.hpp>
using namespace glpp::extra;

#include <gtest/gtest.h>

#include <iostream>
#include <stdexcept>

namespace {
    TEST(VertexTest, Vertex) {
        Vertex v;
        EXPECT_EQ(glm::vec3(0), v.pos);
        EXPECT_EQ(glm::vec3(0), v.norm);
        EXPECT_EQ(glm::vec2(0), v.uv);
    }

    TEST(VertexTest, Vertex_pos_norm_uv) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v(pos, norm, uv);

        EXPECT_EQ(pos, v.pos);
        EXPECT_EQ(norm, v.norm);
        EXPECT_EQ(uv, v.uv);
    }

    TEST(VertexTest, Vertex_Copy) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v(pos, norm, uv);
        Vertex v2(v);

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST(VertexTest, Copy_Assign) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v(pos, norm, uv);
        Vertex v2 = v;

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST(VertexTest, Vertex_Move) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v(pos, norm, uv);
        Vertex v2(std::move(v));

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST(VertexTest, Move_Assign) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v(pos, norm, uv);
        Vertex v2 = std::move(v);

        v.pos.x++;
        v.norm.x++;
        v.uv.x++;

        EXPECT_EQ(pos, v2.pos);
        EXPECT_EQ(norm, v2.norm);
        EXPECT_EQ(uv, v2.uv);
    }

    TEST(VertexTest, Add) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        Vertex v3 = v + v2;

        EXPECT_EQ(glm::vec3(2), v3.pos);
        EXPECT_EQ(glm::vec3(4), v3.norm);
        EXPECT_EQ(glm::vec2(6), v3.uv);
    }

    TEST(VertexTest, Subtract) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        Vertex v3 = v - v2;

        EXPECT_EQ(glm::vec3(0), v3.pos);
        EXPECT_EQ(glm::vec3(0), v3.norm);
        EXPECT_EQ(glm::vec2(0), v3.uv);
    }

    TEST(VertexTest, Add_Assign) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        v2 += v;

        EXPECT_EQ(glm::vec3(2), v2.pos);
        EXPECT_EQ(glm::vec3(4), v2.norm);
        EXPECT_EQ(glm::vec2(6), v2.uv);
    }

    TEST(VertexTest, Subtract_Assign) {
        glm::vec3 pos(1, 2, 3);
        glm::vec3 norm(4, 5, 6);
        glm::vec2 uv(7, 8);

        Vertex v({1, 1, 1}, {2, 2, 2}, {3, 3});
        Vertex v2(v);

        v2 -= v;

        EXPECT_EQ(glm::vec3(0), v2.pos);
        EXPECT_EQ(glm::vec3(0), v2.norm);
        EXPECT_EQ(glm::vec2(0), v2.uv);
    }
}