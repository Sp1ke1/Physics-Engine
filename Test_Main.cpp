#include <gtest/gtest.h>
#include "raylib.h"
#include "Collision.hpp"

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

TEST ( Collision, SphereBoxCollision ) 
{
    Vector3 SphereCenter { 0.f, 0.f, 0.f };
    float SphereRadius = 1.f;

    const BoundingBox BBox1 = { .min = { 0.5f, 0.5f, 0.5f }, .max = { 2.f, 2.f, 2.f } }; // box overlaps sphere
    const BoundingBox BBox2 = { .min = { -2.f, -2.f, -2.f }, .max = { 0.5f, 0.5f, 0.5f } }; // box overlaps sphere
    const BoundingBox BBox3 = { .min = { -1.f, -1.f, -1.f }, .max = { 1.f, 1.f, 1.f } }; // box contains sphere center and overlaps

    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox1 );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox2 );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox3 );
    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_TRUE ( HitResult2 . IsHit );
    EXPECT_TRUE ( HitResult3 . IsHit );
}