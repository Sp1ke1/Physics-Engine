#pragma once 
#include "raylib.h"
#include "Box.hpp"
#include "Ball.hpp"


namespace PE
{
namespace Collision 
{

    struct SHitResult 
    {
        bool IsHit = false; 
        Vector3 Normal { 0.f, 0.f, 0.f }; 
        float Penetration = 0.f; 
    };
    
    SHitResult TestSphereBox ( const Vector3 & SphereCenterLocation, float SphereRadius, const BoundingBox & Box );
    Vector3 ClosestPointOnBox (const Vector3& PointLocation, const BoundingBox & Box );
}

    namespace detail 
    {

    }
} // namespace Collision
} // namespace PE