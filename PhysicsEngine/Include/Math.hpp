#include "raylib.h"

namespace PE
{
namespace Math
{
    /**
     * @brief A very small epsilon used for floating point comparisons.
     */
    constexpr const float GSmallNumber = 1e-12f;

    /**
     * @brief A sma ll epsilon commonly used for near-zero tests in physics.
     */
    constexpr const float GKindaSmallNumber = 1e-6f;
    
    /**
     * @brief Return the closest point on an axis-aligned bounding box to a given point.
     * @param PointLocation point in world coordinates
     * @param Box axis-aligned bounding box
     * @return closest point on the box surface (or interior) to PointLocation
     */
    Vector3 ClosestPointOnBox(const Vector3 &PointLocation, const BoundingBox &Box);

    /**
     * @brief Compute the center point of a bounding box.
     * @param Box axis-aligned bounding box
     * @return center point in world coordinates
     */
    Vector3 BoxCenter ( const BoundingBox & Box );

    /**
     * @brief Compute the size (width/height/depth) of a bounding box.
     * @param Box axis-aligned bounding box
     * @return size vector (max - min)
     */
    Vector3 BoxSize ( const BoundingBox & Box );

    /**
     * @brief Linearly interpolate between two colors.
     * @param C1 start color
     * @param C2 end color
     * @param T interpolation parameter in [0,1]
     * @return interpolated color
     */
    Color ColorLerp ( const Color & C1, const Color & C2, float T ); 
}    
}