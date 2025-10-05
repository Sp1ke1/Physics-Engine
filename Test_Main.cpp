#include <gtest/gtest.h>
#include "raylib.h"
#include "Collision.hpp"

TEST ( Collision, SphereBoxNoCollisionIsHit ) 
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

TEST ( Collision, SphereBoxCollisionIsHit ) 
{
    Vector3 SphereCenter { 0.f, 0.f, 0.f };
    float SphereRadius = 1.f;

    const BoundingBox BBox1 = { .min = { 0.5f, 0.5f, 0.5f }, .max = { 2.f, 2.f, 2.f } }; // box overlaps sphere
    const BoundingBox BBox2 = { .min = { -2.f, -2.f, -2.f }, .max = { 0.5f, 0.5f, 0.5f } }; // box overlaps sphere
    const BoundingBox BBox3 = { .min = { -1.f, -1.f, -1.f }, .max = { 1.f, 1.f, 1.f } }; // box contains sphere center and overlaps
    const BoundingBox BBox4 = { .min = { 1.f, -1.f, -1.f }, .max = { 2.f, 1.f, 1.f } }; // touching


    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox1 );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox2 );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox3 );
    const PE::Collision::SHitResult HitResult4 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox4 );

    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_TRUE ( HitResult2 . IsHit );
    EXPECT_TRUE ( HitResult3 . IsHit );
    EXPECT_TRUE ( HitResult4 . IsHit );
}

TEST ( Collision, SphereSphereNoCollisionIsHit ) 
{
    const Vector3 CenterA { 0.f, 0.f, 0.f };
    const float RadiusA = 1.f; 

    const Vector3 CenterB1 { 5.f, 0.f, 0.f }; // far away
    const Vector3 CenterB2 { 2.1f, 0.f, 0.f }; // just outside
    const Vector3 CenterB3 { -5.f, -5.f, -5.f }; // far away

    const float RadiusB = 1.f; 

    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB1, RadiusB );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB2, RadiusB );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB3, RadiusB );
    EXPECT_FALSE ( HitResult1 . IsHit ); 
    EXPECT_FALSE ( HitResult2 . IsHit );
    EXPECT_FALSE ( HitResult3 . IsHit );
}

TEST ( Collision, SphereSphereCollisionIsHit ) 
{
    const Vector3 CenterA { 0.f, 0.f, 0.f };
    const float RadiusA = 1.f; 

    const Vector3 CenterB1 { 2.f, 0.f, 0.f }; // just touching
    const Vector3 CenterB2 { 1.f, 0.f, 0.f }; // overlapping
    const Vector3 CenterB3 { 0.5f, 0.5f, 0.5f }; // overlapping

    const float RadiusB = 1.f; 
    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB1, RadiusB );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB2, RadiusB );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB3, RadiusB );
    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_TRUE ( HitResult2 . IsHit );
    EXPECT_TRUE ( HitResult3 . IsHit );
}

TEST ( Collision, SphereBoxPenetration ) 
{
    const Vector3 SphereCenter { 0.f, 1.f, 0.f };
    const float SphereRadius = 1.f; 

    const BoundingBox BBox1 = { .min = { -1.f, -1.f, -1.f }, .max = { 0.f, 0.1f, 0.f } };
    const BoundingBox BBox2 = { .min = { -1.f, -1.f, -1.f }, .max = { 0.f, 0.5f, 0.f } };


    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox1 );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox2 );
    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult1 . Penetration, 0.1f  );
    EXPECT_FLOAT_EQ ( HitResult2 . Penetration, 0.5f  );
    EXPECT_TRUE ( HitResult2 . IsHit );
}

TEST ( Collision, SphereSpherePenetration )
{
    const Vector3 CenterA { 0.f, 0.f, 0.f };
    const float RadiusA = 1.f; 

    const Vector3 CenterB1 { 1.5f, 0.f, 0.f }; // overlapping by 0.5
    const Vector3 CenterB2 { 1.f, 0.f, 0.f }; // overlapping by 1.0
    const Vector3 CenterB3 { 0.5f, 0.f, 0.f }; // overlapping by 1.5

    const float RadiusB = 1.f; 
    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB1, RadiusB );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB2, RadiusB );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB3, RadiusB );
    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult1 . Penetration, 0.5f );
    EXPECT_TRUE ( HitResult2 . IsHit ); 
    EXPECT_FLOAT_EQ ( HitResult2 . Penetration, 1.0f );
    EXPECT_TRUE ( HitResult3 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult3 . Penetration, 1.5f );
}

TEST ( Collision, SphereBoxContactPointAndNormal ) 
{
    const Vector3 SphereCenter { 0.f, 0.f, 0.f };
    const float SphereRadius = 1.f; 

    const BoundingBox BBox1 = { . min = { -2.f, -2.f, -2.f }, . max = { 2.f, -1.f, 2.f } }; // box below sphere
    const BoundingBox BBox2 = { . min = { -2.f, 1.f, -2.f }, . max = { 2.f, 2.f, 2.f } }; // box above sphere

    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox1 );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereBox ( SphereCenter, SphereRadius, BBox2 );
    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult1 . Penetration, 0.0f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . y, 1.f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . z, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . y, -1.f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . z, 0.f );

    EXPECT_TRUE ( HitResult2 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult2 . Penetration, 0.0f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . y, -1.f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . z, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . y, 1.f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . z, 0.f );
}

TEST( Collision, SphereSphereContactPointAndNormal ) 
{
    const Vector3 CenterA { 0.f, 0.f, 0.f };
    const float RadiusA = 1.f; 

    const Vector3 CenterB1 { 1.5f, 0.f, 0.f }; // overlapping by 0.5
    const Vector3 CenterB2 { -1.5f, 0.f, 0.f }; // overlapping by 0.5
    const Vector3 CenterB3 { 0.f, 1.5f, 0.f }; // overlapping by 0.5

    const float RadiusB = 1.f; 
    const PE::Collision::SHitResult HitResult1 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB1, RadiusB );
    const PE::Collision::SHitResult HitResult2 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB2, RadiusB );
    const PE::Collision::SHitResult HitResult3 = PE::Collision::TestSphereSphere ( CenterA, RadiusA, CenterB3, RadiusB );

    EXPECT_TRUE ( HitResult1 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult1 . Penetration, 0.5f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . x, -1.f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . y, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . Normal . z, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . x, 0.75f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . y, 0.f );
    EXPECT_FLOAT_EQ ( HitResult1 . ContactPoint . z, 0.f );
    
    EXPECT_TRUE ( HitResult2 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult2 . Penetration, 0.5f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . x, 1.f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . y, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . Normal . z, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . x, -0.75f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . y, 0.f );
    EXPECT_FLOAT_EQ ( HitResult2 . ContactPoint . z, 0.f );
    
    EXPECT_TRUE ( HitResult3 . IsHit );
    EXPECT_FLOAT_EQ ( HitResult3 . Penetration, 0.5f );
    EXPECT_FLOAT_EQ ( HitResult3 . Normal . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult3 . Normal . y, -1.f );
    EXPECT_FLOAT_EQ ( HitResult3 . Normal . z, 0.f );
    EXPECT_FLOAT_EQ ( HitResult3 . ContactPoint . x, 0.f );
    EXPECT_FLOAT_EQ ( HitResult3 . ContactPoint . y, 0.75f );
    EXPECT_FLOAT_EQ ( HitResult3 . ContactPoint . z, 0.f );

}