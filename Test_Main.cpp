#include <gtest/gtest.h>
#include "raylib.h"
#include "Collision.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST ( Collision, SphereBoxNoCollision ) 
{
    const Vector3 SphereCenter = { 0.f, 0.f, 0.f };
    const float SphereRadius = 1.f;

    const BoundingBox BBox = { .min = { 5.f, 5.f, 5.f }, .max = { 10.f, 10.f, 10.f } };
    const PE::Collision::SHitResult HitResult = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox );
    EXPECT_FALSE ( HitResult . IsHit );
}