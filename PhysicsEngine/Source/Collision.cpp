#include "Collision.hpp"
#include "Math.hpp"
#include "raymath.h"

namespace PE 
{
namespace Collision 
{
    SHitResult TestCollision(const SPhysicsBody &BodyA, const SPhysicsBody &BodyB)
    {
        if ( BodyA . Shape . Type == EShapeType::Sphere && BodyB . Shape . Type == EShapeType::Sphere )
        {
            return TestSphereSphere ( BodyA . Position, BodyA . Shape . Sphere . Radius, 
                                     BodyB . Position, BodyB . Shape . Sphere . Radius ); 
        }
        else if ( BodyA . Shape . Type == EShapeType::Box && BodyB . Shape . Type == EShapeType::Sphere )
        {
            const BoundingBox Box = { 
                Vector3Subtract ( BodyA . Position, BodyA . Shape . Box . HalfSize ), 
                Vector3Add    ( BodyA . Position, BodyA . Shape . Box . HalfSize ) }; 
            SHitResult HitResult = TestSphereBox ( BodyB . Position, BodyB . Shape . Sphere . Radius, Box ); 
            // Invert normal to point from B to A
            HitResult.Normal = Vector3Negate ( HitResult . Normal );
            return HitResult; 
        }
        else if ( BodyA . Shape . Type == EShapeType::Sphere && BodyB . Shape . Type == EShapeType::Box )
        {
            const BoundingBox Box = { 
                Vector3Subtract ( BodyB . Position, BodyB . Shape . Box . HalfSize ), 
                Vector3Add    ( BodyB . Position, BodyB . Shape . Box . HalfSize ) }; 
            return TestSphereBox ( BodyA . Position, BodyA . Shape . Sphere . Radius, Box );
        }
        else 
        {
            // Box-Box not implemented
            SHitResult OutHitResult; 
            OutHitResult.IsHit = false; 
            return OutHitResult; 
        }
    }

    SHitResult TestSphereBox( const Vector3 SphereCenter, float SphereRadius, const BoundingBox & Box )
    {
        SHitResult OutHitResult; 
        const Vector3 ClosestOnBox = PE::Math::ClosestPointOnBox( SphereCenter, Box );
        const float DistanceSquared = Vector3DistanceSqr( SphereCenter, ClosestOnBox );
        const float RadiusSquared = SphereRadius * SphereRadius;
        // No overlap (treat exact touching as a hit — only strictly greater means no collision)
        if ( DistanceSquared > RadiusSquared )
        {
            OutHitResult . IsHit = false; 
            return OutHitResult; 
        }
        
        // Center is out of the box
        if ( DistanceSquared > PE::Math::GSmallNumber )
        {
            const Vector3 Direction = Vector3Subtract ( SphereCenter, ClosestOnBox );
            const float Distance = sqrtf ( DistanceSquared ); 
            OutHitResult . IsHit = true;
            OutHitResult . Normal = Vector3Normalize ( Direction );
            OutHitResult . Penetration = SphereRadius - Distance;
            const Vector3 PointOnSphere = Vector3Subtract( SphereCenter, Vector3Scale( OutHitResult.Normal, SphereRadius ) );
            OutHitResult . ContactPoint = Vector3Scale( Vector3Add( ClosestOnBox, PointOnSphere ), 0.5f );
            return OutHitResult; 
        }

        // Center is inside the box 
        const float DistanceXMin = SphereCenter . x - Box.min.x;
        const float DistanceXMax = Box.max.x - SphereCenter . x ;   
        const float DistanceYMin = SphereCenter . y - Box.min.y;
        const float DistanceYMax = Box.max.y - SphereCenter . y; 
        const float DistanceZMin = SphereCenter . z - Box.min.z;
        const float DistanceZMax = Box.max.z - SphereCenter . z; 

        float DistanceMin = DistanceXMin; 
        Vector3 HitNormal { -1, 0, 0 };

        if ( DistanceXMax < DistanceMin ) { DistanceMin = DistanceXMax; HitNormal = { +1, 0, 0 }; }
        if ( DistanceYMin < DistanceMin ) { DistanceMin = DistanceYMin; HitNormal = { 0, -1, 0 }; }
        if ( DistanceYMax < DistanceMin ) { DistanceMin = DistanceYMax; HitNormal = { 0, +1, 0 }; }
        if ( DistanceZMin < DistanceMin ) { DistanceMin = DistanceZMin; HitNormal = { 0, 0, -1 }; }
        if ( DistanceZMax < DistanceMin ) { DistanceMin = DistanceZMax; HitNormal = { 0, 0, +1 }; }
        
        const Vector3 PointOnBox = Vector3Add( SphereCenter, Vector3Scale( HitNormal, DistanceMin ) );
        const Vector3 PointOnSphere = Vector3Add( SphereCenter, Vector3Scale( HitNormal, SphereRadius ) );
        OutHitResult . IsHit = true;
        OutHitResult . Normal = HitNormal; 
        OutHitResult . Penetration = SphereRadius - DistanceMin;
        OutHitResult . ContactPoint = Vector3Scale( Vector3Add( PointOnBox, PointOnSphere ), 0.5f );
        return OutHitResult;
    }

    SHitResult TestSphereSphere(const Vector3 &SphereCenterA, float SphereRadiusA, const Vector3 &SphereCenterB, float SphereRadiusB)
    {
   
        SHitResult OutHitResult; 
        const Vector3 Direction = Vector3Subtract( SphereCenterA, SphereCenterB );
        const float   DistanceSquared = Vector3DistanceSqr ( SphereCenterB, SphereCenterA );
        const float   RadiiSum = SphereRadiusA + SphereRadiusB;
        const float   RadiiSumSquared = RadiiSum * RadiiSum;

        // No overlap (treat exact touching as a hit)
        if ( DistanceSquared > RadiiSumSquared)
        {
            return OutHitResult;
        }

        OutHitResult . IsHit = true;
        const float Distance = sqrtf(DistanceSquared);

        // Centers are the same 
        if (Distance < PE::Math::GSmallNumber)
        {
            OutHitResult.Normal = {1.f, 0.f, 0.f}; // Random normal
            OutHitResult.Penetration = RadiiSum; // Maximum penetration
            OutHitResult.ContactPoint = SphereCenterA; 
            return OutHitResult;
        }

        // Overlap 
        OutHitResult . Normal = Vector3Normalize ( Direction ); 
        OutHitResult . Penetration = RadiiSum - Distance;
        
        // Contact point = midpoint between surface points along the normal
        const Vector3 ContactPointOffsetAlongNormal = Vector3Scale(OutHitResult.Normal, SphereRadiusA - 0.5f * OutHitResult.Penetration);
        OutHitResult.ContactPoint = Vector3Add(SphereCenterB, ContactPointOffsetAlongNormal);
        return OutHitResult; 
    }

} // namespace Collision
} // namespace PE