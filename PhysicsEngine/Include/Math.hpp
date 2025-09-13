#include "raylib.h"

namespace PE
{
namespace Math
{
    Vector3 ClosestPointOnBox(const Vector3 &PointLocation, const BoundingBox &Box);
    Vector3 BoxCenter ( const BoundingBox & Box );
    Vector3 BoxSize ( const BoundingBox & Box );
}    
}