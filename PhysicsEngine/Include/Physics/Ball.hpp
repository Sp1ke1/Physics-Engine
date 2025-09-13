#pragma once
#include "raylib.h"

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