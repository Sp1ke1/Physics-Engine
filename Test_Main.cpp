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
    Vector3 SphereCenter { 0.f, 0.f, 0.f };
    float SphereRadius = 1.f;

    const BoundingBox BBox1 = { .min = { 5.f, 5.f, 5.f }, .max = { 10.f, 10.f, 10.f } }; // box is far away
    const BoundingBox BBox2 = { .min = { -10.f, -10.f, -10.f }, .max = { -5.f, -5.f, -5.f } }; // box is far away
    const BoundingBox BBox3 = { .min = { -2.f, -2.f, -2.f }, .max = { -1.f, -1.f, -1.f } }; // box contains sphere center but no overlap


    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox1 );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox2 );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox3 );
    EXPECT_FALSE ( HitResult1 . IsHit );
    EXPECT_FALSE ( HitResult2 . IsHit );
    EXPECT_FALSE ( HitResult3 . IsHit );

}