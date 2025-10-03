#pragma once
#include "raylib.h"

enum class EShapeType : short
{
    Box,
    Sphere,
};

struct SShape
{
    EShapeType Type;
    union
    {
        struct
        {
            Vector3 HalfSize; 
        } Box;
        
        struct
        {
            float Radius;
        } Sphere;
    };

    float GetMomentOfInertia(float Mass) const
    {
        switch (Type)
        {
            case EShapeType::Box:
                return (1.f / 12.f) * Mass * ( Box.HalfSize.x * Box.HalfSize.x + Box.HalfSize.y * Box.HalfSize.y + 
                                               Box.HalfSize.y * Box.HalfSize.y + Box.HalfSize.z * Box.HalfSize.z + 
                                               Box.HalfSize.z * Box.HalfSize.z + Box.HalfSize.x * Box.HalfSize.x );
            case EShapeType::Sphere:
                return (2.f / 5.f) * Mass * Sphere.Radius * Sphere.Radius;
            default:
                return 0.f;
        }
    }
};