#include "Collision.hpp"
#include "raymath.h"
#include "Misc.hpp"

namespace PE 
{
namespace Collision 
{
   
    SHitResult TestSphereBox( const Vector3 & SphereCenter, float SphereRadius, const BoundingBox & Box )
    {
        SHitResult OutHitResult; 
        const Vector3 ClosestOnBox = ClosestPointOnBox( SphereCenter, Box );
        const float DistanceSquared = Vector3DistanceSqr( SphereCenter, ClosestOnBox );
        const float RadiusSquared = SphereRadius * SphereRadius;
        // No overlap 
        if ( DistanceSquared > RadiusSquared )
        {
            OutHitResult . IsHit = false; 
            return OutHitResult;   
        }
        
        // Center is out of the box
        if ( DistanceSquared > GSmallNumber );
        {
            const Vector3 Direction = Vector3Subtract ( SphereCenter, ClosestOnBox );
            const float Distance = sqrtf ( DistanceSquared ); 
            OutHitResult . IsHit = true;
            OutHitResult . Normal = Vector3Normalize ( Direction );
            return OutHitResult; 
        }

        // Center is inside the box 
        const float DistanceXMin = SphereCenter.x - Box.min.x;
        const float DistanceXMax = Box.max.x - SphereCenter.x; 
        const float DistanceYMin = SphereCenter.y - Box.min.y;
        const float DistanceYMax = Box.max.y - SphereCenter.y; 
        const float DistanceZMin = SphereCenter.z - Box.min.z;
        const float DistanceZMax = Box.max.z - SphereCenter.z; 

        float DistanceMin = DistanceXMin; 
        Vector3 HitNormal { -1, 0, 0 };

        if ( DistanceXMax < DistanceMin ) { DistanceMin = DistanceXMax; HitNormal = { +1, 0, 0 }; }
        if ( DistanceYMin < DistanceMin ) { DistanceMin = DistanceYMin; HitNormal = { 0, -1, 0 }; }
        if ( DistanceYMax < DistanceMin ) { DistanceMin = DistanceYMax; HitNormal = { 0, +1, 0 }; }
        if ( DistanceZMin < DistanceMin ) { DistanceMin = DistanceZMin; HitNormal = { 0, 0, -1 }; }
        if ( DistanceZMax < DistanceMin ) { DistanceMin = DistanceZMax; HitNormal = { 0, 0, +1 }; }
        
        if ( DistanceMin < SphereRadius ) 
        {
            OutHitResult . IsHit = true;
            OutHitResult . Normal = HitNormal; 
            OutHitResult . Penetration = SphereRadius - DistanceMin;
            return OutHitResult; 
        }
        
        // Sphere is inside with no overlap 
        return OutHitResult; 
    }
    Vector3 ClosestPointOnBox(const Vector3 &PointLocation, const BoundingBox &Box)
    {
        Vector3 OutPoint; 
        OutPoint.x = Clamp(PointLocation.x, Box.min.x, Box.max.x);
        OutPoint.y = Clamp(PointLocation.y, Box.min.y, Box.max.y);
        OutPoint.z = Clamp(PointLocation.z, Box.min.z, Box.max.z);
        return OutPoint;
    }
} // namespace Collision
} // namespace PE