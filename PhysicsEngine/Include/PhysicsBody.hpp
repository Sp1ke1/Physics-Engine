#pragma once
#include "raylib.h"
#include "Shape.hpp"


namespace PE
{
   struct SPhysicsBody
    {
        SShape Shape; 
        
        Quaternion Rotation{ 0.f, 0.f, 0.f };
        
        Vector3 Position{ 0.f, 0.f, 0.f };
        Vector3 LinearVelocity{ 0.f, 0.f, 0.f };
        Vector3 AngularVelocity{ 0.f, 0.f, 0.f };

        float Mass = 1.0f;
        float InvMass = 1.0f;
        
        float Restitution = 0.5f;
        float Friction = 0.5f;
        float AngularDamping = 0.98f;
        float LinearDamping = 0.98f;
        
        int Id = -1; 

        bool IsStatic = false;
    };
} // namespace PE