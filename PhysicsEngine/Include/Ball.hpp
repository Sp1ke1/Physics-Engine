#pragma once
#include "raylib.h"

namespace PE 
{
    struct SBall 
    {
        Vector3 Center; 
        float Radius; 
        Quaternion Rotation; 
        Vector3 LinearVelocity; 
        Vector3 AngularVelocity;
        float Mass;
        Color Color;
    };
} // namespace PE