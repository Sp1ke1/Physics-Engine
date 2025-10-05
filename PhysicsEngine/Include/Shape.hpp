#pragma once
#include "raylib.h"

/**
 * @brief Supported primitive shape types.
 */
enum class EShapeType : short
{
    Box,
    Sphere,
};

/**
 * @brief Simple shape union for primitives used by physics bodies.
 *
 * Contains only the essential geometric parameters for box and sphere.
 */
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

    /**
     * @brief Return scalar moment of inertia for the shape given mass.
     *
     * This is a simplified scalar result (not a full inertia tensor).
     */
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