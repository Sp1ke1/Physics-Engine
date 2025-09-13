#pragma once 
#include "raylib.h"

namespace PE 
{
struct SBox 
{
    Vector3 Min; 
    Vector3 Max; 
    Color Color;

    Vector3 Size() const { return { Max.x - Min.x, Max.y - Min.y, Max.z - Min.z }; }
    Vector3 Center() const { return { (Min.x + Max.x) /2, (Min.y + Max.y) /2, (Min.z + Max.z) /2 }; }
};
} // namespace PE