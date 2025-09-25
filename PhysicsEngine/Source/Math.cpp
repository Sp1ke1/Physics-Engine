#include "Math.hpp"
#include "raymath.h"


namespace PE
{
namespace Math
{
    Vector3 PE::Math::ClosestPointOnBox(const Vector3 &PointLocation, const BoundingBox &Box)
    {
        Vector3 OutPoint; 
        OutPoint.x = Clamp(PointLocation.x, Box.min.x, Box.max.x);
        OutPoint.y = Clamp(PointLocation.y, Box.min.y, Box.max.y);
        OutPoint.z = Clamp(PointLocation.z, Box.min.z, Box.max.z);
        return OutPoint;
    }
    
    Vector3 PE::Math::BoxCenter(const BoundingBox &Box)
    {
        return { 0.5f * ( Box.min.x + Box.max.x ), 
                    0.5f * ( Box.min.y + Box.max.y ), 
                    0.5f * ( Box.min.z + Box.max.z ) }; 
    }
    
    Vector3 PE::Math::BoxSize(const BoundingBox &Box)
    {
        return Vector3Subtract ( Box . max, Box . min );
    }

    Color ColorLerp(const Color &C1, const Color &C2, float T)
    {
        if (T < 0.0f)
        {
            T = 0.0f;
        }
            
        if (T > 1.0f)
        {
            T = 1.f; 
        }
        return {
            (unsigned char)(C1.r + (C2.r - C1.r) * T),
            (unsigned char)(C1.g + (C2.g - C1.g) * T),
            (unsigned char)(C1.b + (C2.b - C1.b) * T),
            (unsigned char)(C1.a + (C2.a - C1.a) * T)
        };
    }

} // namespace Math
} // namespace PE
