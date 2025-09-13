#include "Collision.hpp"
#include "Misc.hpp"
#include "Math.hpp"
#include "raymath.h"

namespace PE 
{
namespace Collision 
{
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
        if ( DistanceSquared > GSmallNumber )
        {
            const Vector3 Direction = Vector3Subtract ( SphereCenter, ClosestOnBox );
            const float Distance = sqrtf ( DistanceSquared ); 
            OutHitResult . IsHit = true;
            OutHitResult . Normal = Vector3Normalize ( Direction );
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
    
        OutHitResult . IsHit = true;
        OutHitResult . Normal = HitNormal; 
        OutHitResult . Penetration = SphereRadius - DistanceMin;
        return OutHitResult;
    }

    SHitResult TestSphereSphere(const Vector3 &SphereCenterA, float SphereRadiusA, const Vector3 &SphereCenterB, float SphereRadiusB)
    {
   
        SHitResult OutHitResult; 
        const Vector3 Direction = Vector3Subtract( SphereCenterB, SphereCenterA );
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
        if (Distance < GSmallNumber)
        {
            OutHitResult.Normal = {1.f, 0.f, 0.f}; // Random normal
            OutHitResult.Penetration = RadiiSum; // Maximum penetration
            return OutHitResult;
        }

        // Overlap 
        OutHitResult . Normal = Vector3Normalize ( Direction ); 
        OutHitResult . Penetration = RadiiSum - Distance;
        return OutHitResult; 
    }

} // namespace Collision
} // namespace PE