#pragma once 
#include "raylib.h"


namespace PE
{
namespace Collision 
{

    struct SHitResult 
    {
        bool IsHit = false; 
        Vector3 Normal { 0.f, 0.f, 0.f }; 
        float Penetration = 0.f; 
        Vector3 ContactPointOnA; 
        Vector3 ContactPointOnB; 
    };
    
    SHitResult TestSphereBox ( const Vector3 SphereCenter, float SphereRadius, const BoundingBox & Box );
    SHitResult TestSphereSphere ( const Vector3 & SphereCenterA, float SphereRadiusA, const Vector3 & SphereCenterB, float SphereRadiusB );
    Vector3 ClosestPointOnBox (const Vector3 & PointLocation, const BoundingBox & Box );
} // namespace Collision
} // namespace PE