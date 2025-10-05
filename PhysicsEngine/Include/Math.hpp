#include "raylib.h"

namespace PE
{
namespace Math
{
    constexpr float GSmallNumber = 1e-12f;
    constexpr float GKindaSmallNumber = 1e-6f;
    
    Vector3 ClosestPointOnBox(const Vector3 &PointLocation, const BoundingBox &Box);
    Vector3 BoxCenter ( const BoundingBox & Box );
    Vector3 BoxSize ( const BoundingBox & Box );
    Color ColorLerp ( const Color & C1, const Color & C2, float T ); 
}    
}