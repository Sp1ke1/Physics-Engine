#pragma once
#include "raylib.h"

namespace PE 
{

    struct SBall 
    {
        Vector3 Location; 
        Quaternion Rotation; 
        Vector3 LinearVelocity; 
        Vector3 AngularVelocity;
        float Radius; 
        float Mass; 
        Color Color;  
    };
} // namespace PE