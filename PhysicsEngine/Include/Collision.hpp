#pragma once 
#include "Shape.hpp"
#include "PhysicsBody.hpp"
#include "raylib.h"


namespace PE
{
namespace Collision 
{

    /**
     * @brief Result of a collision test.
     *
     * Contains contact point, contact normal (points from B toward A in TestCollision),
     * penetration depth and a hit flag.
     */
    struct SHitResult 
    {
        Vector3 ContactPoint { 0.f, 0.f, 0.f }; 
        Vector3 Normal { 0.f, 0.f, 0.f }; 
        float Penetration = 0.f; 
        bool IsHit = false; 
    };
    
    /**
     * @brief Generic collision test between two physics bodies.
     * @param BodyA first physics body
     * @param BodyB second physics body
     * Supports sphere-sphere and sphere-box pairs. Returns SHitResult with
     * contact information when a collision is detected.
     */
    SHitResult TestCollision ( const SPhysicsBody & BodyA, const SPhysicsBody & BodyB ); 

    /**
     * @brief Test collision between a sphere and an axis-aligned bounding box.
     * @param SphereCenter center of the sphere in world coordinates
     * @param SphereRadius radius of the sphere
     * @param Box axis-aligned bounding box to test against
     * @return SHitResult containing contact information when a collision occurs
     */
    SHitResult TestSphereBox ( const Vector3 SphereCenter, float SphereRadius, const BoundingBox & Box );

    /**
     * @brief Test collision between two spheres.
     * @param SphereCenterA center of the first sphere in world coordinates
     * @param SphereRadiusA radius of the first sphere
     * @param SphereCenterB center of the second sphere in world coordinates
     * @param SphereRadiusB radius of the second sphere
     * @return SHitResult containing contact information when a collision occurs
     */
    SHitResult TestSphereSphere ( const Vector3 & SphereCenterA, float SphereRadiusA, const Vector3 & SphereCenterB, float SphereRadiusB );
} // namespace Collision
} // namespace PE