#pragma once
#include "raylib.h"
#include "Shape.hpp"


namespace PE
{
   struct SPhysicsBody
    {
        Vector3 Position{ 0.f, 0.f, 0.f };
        Quaternion Rotation{ 0.f, 0.f, 0.f };
        Vector3 LinearVelocity{ 0.f, 0.f, 0.f };
        Vector3 AngularVelocity{ 0.f, 0.f, 0.f };

        SShape Shape; 

        float Mass = 1.0f;
        float InvMass = 1.0f;
        bool IsStatic = false;
        
        float Restitution = 0.5f;
        float Friction = 0.5f;
        float AngularDamping = 0.98f;
        float LinearDamping = 0.98f;

        int Id = -1; 
    };
} // namespace PE