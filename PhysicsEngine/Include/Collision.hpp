#pragma once 
#include "Shape.hpp"
#include "PhysicsBody.hpp"
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
        Vector3 ContactPoint { 0.f, 0.f, 0.f }; 
    };
    
    SHitResult TestCollision ( const SPhysicsBody & BodyA, const SPhysicsBody & BodyB ); 
    SHitResult TestSphereBox ( const Vector3 SphereCenter, float SphereRadius, const BoundingBox & Box );
    SHitResult TestSphereSphere ( const Vector3 & SphereCenterA, float SphereRadiusA, const Vector3 & SphereCenterB, float SphereRadiusB );
    Vector3 ClosestPointOnBox (const Vector3 & PointLocation, const BoundingBox & Box );
} // namespace Collision
} // namespace PE