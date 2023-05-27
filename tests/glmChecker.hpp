#pragma once

#include <gtest/gtest-assertion-result.h>

#include <glm/glm.hpp>

testing::AssertionResult GLMEqual(const glm::vec3 & a, const glm::vec3 & b);

testing::AssertionResult GLMEqual(const glm::quat & a, const glm::quat & b);

testing::AssertionResult GLMEqual(const glm::mat4 & a, const glm::mat4 & b);

#define EXPECT_GLM_EQUAL(A, B) EXPECT_TRUE(GLMEqual((A), (B)))
#define EXPECT_GLM_NEQUAL(A, B) EXPECT_FALSE(GLMEqual((A), (B)))
