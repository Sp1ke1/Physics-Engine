#pragma once
#include "Scene.hpp"
#include "Collision.hpp"
#include "raymath.h"
#include "Math.hpp"
#include <iostream>

namespace PE 
{
    CScene::CScene( const SSceneParameters & SceneParameters )
    {
        Initialize( SceneParameters );
    }

    void CScene::Update(float DeltaTime) 
    {
        UpdateCamera ( &m_Camera, CAMERA_FREE );

        m_TimeAccumulator += DeltaTime;
        while ( m_TimeAccumulator >= m_FixedDeltaTime )
        {
            SimulationStep( m_FixedDeltaTime );
            m_TimeAccumulator -= m_FixedDeltaTime;
        }
    }
    void CScene::SetWindowParameters(const SWindowParameters &WindowParameters)
    {
        SetWindowSize ( WindowParameters.ScreenWidth, WindowParameters . ScreenHeight );
        SetTargetFPS ( WindowParameters . TargetFPS );
    }
    void CScene::SetCameraParameters(const SCameraParameters &CameraParameters)
    {
        m_Camera . position = CameraParameters . Position;
        m_Camera . target = CameraParameters . Target;
        m_Camera . up = CameraParameters . Up;
        m_Camera . fovy = CameraParameters . FovY;
        m_Camera . projection = CameraParameters . Projection;
    }

    void CScene::Draw()
    {
        ClearBackground(RAYWHITE);
        BeginMode3D(m_Camera);
            DrawWorld();
            DrawBalls(); 
        EndMode3D();
        DrawTimer(); 
    }
    void CScene::Initialize( const SSceneParameters & SceneParameters )
    {
        InitWindow ( SceneParameters . WindowParameters . ScreenWidth, SceneParameters . WindowParameters . ScreenHeight, SceneParameters . WindowParameters . Title . c_str());
        SetTargetFPS ( SceneParameters . WindowParameters . TargetFPS );
        DisableCursor(); 
        SetCameraParameters ( SceneParameters . CameraParameters );
        SetSimulationParameters ( SceneParameters . SimulationParameters);
        m_SimulationStartTime = GetTime(); 
    }

    void CScene::DrawTimer()
    {
        const double ElapsedTime = GetTime() - m_SimulationStartTime; 
        char Buffer [64];
        snprintf(Buffer, sizeof(Buffer), "Time: %.2f s", ElapsedTime );
        DrawText(Buffer, 0, 0, 20, BLACK);
    }

    void CScene::SetSimulationParameters ( const SSimulationParameters &SimulationParameters)
    {
        m_RandomGenerator = std::mt19937 ( SimulationParameters . RandomSeed );
        m_Balls = GenerateBalls ( SimulationParameters . NumberOfBalls, SimulationParameters . BallGenerationParameters );
        m_Gravity = SimulationParameters . Gravity;
        m_WorldBox = { .min = SimulationParameters . WorldBoxMin, .max = SimulationParameters . WorldBoxMax };
        m_WorldPlanes = BoundingBoxToPlanes ( m_WorldBox );
        m_BallsRestitution = SimulationParameters . BallsRestitution;
        m_FixedDeltaTime = 1.f / static_cast<float> ( SimulationParameters . SimulationFrequency );
        m_Slop = SimulationParameters . Slop;
        m_NumberOfSolverSteps = SimulationParameters . NumberOfSteps;
    }

    void CScene::DrawBall(const SBall &Ball)
    {   
        DrawSphere( Ball . Center, Ball . Radius, Ball . Color );
        DrawSphereWires( Ball . Center, Ball . Radius, 8, 8, BLACK);
    }

    void CScene::DrawBalls()
    {   
        for ( const auto & Ball : m_Balls ) 
        {
            DrawBall ( Ball ); 
        }
    }

    void CScene::DrawWorld()
    {
        DrawBox ( m_WorldBox ); 
    }
    void CScene::SimulationStep (float DeltaTime)
    {
        IntegrateLinear( DeltaTime );
        ResolveCollisions ( DeltaTime );
    }
    void CScene::IntegrateLinear( float DeltaTime )
    {
        for (auto& Ball : m_Balls)
        {
            Ball.LinearVelocity = Vector3Add(Ball.LinearVelocity,
                Vector3Scale({0.f, -m_Gravity, 0.f}, DeltaTime));

            Ball.Center = Vector3Add(Ball.Center,
                Vector3Scale(Ball.LinearVelocity, DeltaTime));
        }
    }
    void CScene::ResolveCollisions(float DeltaTime)
    {
        for ( auto i = 0; i < m_NumberOfSolverSteps; ++i )
        {
            ResolveCollisionsWithWalls ( DeltaTime );
            ResolveCollisionsBetweenBalls ( DeltaTime );
        }
    }
    void CScene::ResolveCollisionsWithWalls(float DeltaTime)
    {
        for (auto& Ball : m_Balls)
        {
            for (const auto& Plane : m_WorldPlanes)
            {
                const PE::Collision::SHitResult Hit = PE::Collision::TestSphereBox(Ball.Center, Ball.Radius, Plane);

                if (Hit.IsHit)
                {
                    // Positional correction
                    const float Penetration = Hit.Penetration - m_Slop;
                    if ( Penetration > 0.f ) 
                    {
                        Ball.Center = Vector3Add(Ball.Center, Vector3Scale(Hit.Normal, Hit.Penetration));
                    }
                    const float vn = Vector3DotProduct(Ball.LinearVelocity, Hit.Normal);
                    if (vn < 0.f)
                    {
                        const Vector3 vN = Vector3Scale(Hit.Normal, vn);
                        Ball.LinearVelocity = Vector3Subtract( Ball.LinearVelocity, Vector3Scale(vN, (1.f + m_BallsRestitution)));
                    }
                }
            }
        }
    }
    void CScene::ResolveCollisionsBetweenBalls(float DeltaTime)
{
    for (size_t i = 0; i < m_Balls . size(); ++i)
    {
        for (size_t j = i + 1; j < m_Balls . size(); ++j)
        {
            SBall& A = m_Balls[i];
            SBall& B = m_Balls[j];

            const PE::Collision::SHitResult Hit = PE::Collision::TestSphereSphere(A.Center, A.Radius, B.Center, B.Radius);

            if (!Hit.IsHit) 
            {
                continue;
            }
            const float InverseMassA = (A.Mass > 0.f) ? 1.0f / A.Mass : 0.f;
            const float InverseMassB = (B.Mass > 0.f) ? 1.0f / B.Mass : 0.f;
            const float InverseMassSum = InverseMassA + InverseMassB;

            if (InverseMassSum > 0.f)
            {
                const float Slop = 1e-5f;
                const float Penetration = (Hit.Penetration > Slop) ? (Hit.Penetration - Slop) : 0.f;

                if (Penetration > 0.f)
                {
                    const Vector3 Correction = Vector3Scale(Hit.Normal, Penetration / InverseMassSum);
                    if (InverseMassA > 0.f) 
                    { 
                        A.Center = Vector3Subtract(A.Center, Vector3Scale(Correction, InverseMassA));
                    }
                    if (InverseMassB > 0.f) 
                    { 
                        B.Center = Vector3Add(B.Center, Vector3Scale(Correction, InverseMassB));
                    }
                }
            }

            // Normal impulse
            const Vector3 RelativeVelocity = Vector3Subtract(B.LinearVelocity, A.LinearVelocity);
            const float VelocityNormal = Vector3DotProduct(RelativeVelocity, Hit.Normal);

            // Apply impulse only if they are moving towards each other
            if (VelocityNormal < 0.f) 
            {
                const float ImpulseScale = -(1.f + m_BallsRestitution) * VelocityNormal / std::max(InverseMassSum, 1e-12f);
                const Vector3 Impulse = Vector3Scale(Hit.Normal, ImpulseScale);

                if (InverseMassA > 0.f) A.LinearVelocity = Vector3Subtract(A.LinearVelocity, Vector3Scale(Impulse, InverseMassA));
                if (InverseMassB > 0.f) B.LinearVelocity = Vector3Add(B.LinearVelocity, Vector3Scale(Impulse, InverseMassB));
            }
        }
    }
}
std::array<BoundingBox, 6> CScene::BoundingBoxToPlanes(const BoundingBox &Box) const
{

    std::array<BoundingBox, 6> OutPlanes{};
    // Order: left, right, bottom, top, front, back

    // left  (x = min)
    OutPlanes[0].min = {Box.min.x, Box.min.y, Box.min.z};
    OutPlanes[0].max = {Box.min.x, Box.max.y, Box.max.z};

    // right (x = max)
    OutPlanes[1].min = {Box.max.x, Box.min.y, Box.min.z};
    OutPlanes[1].max = {Box.max.x, Box.max.y, Box.max.z};

    // bottom (y = min)
    OutPlanes[2].min = {Box.min.x, Box.min.y, Box.min.z};
    OutPlanes[2].max = {Box.max.x, Box.min.y, Box.max.z};

    // top (y = max)
    OutPlanes[3].min = {Box.min.x, Box.max.y, Box.min.z};
    OutPlanes[3].max = {Box.max.x, Box.max.y, Box.max.z};

    // front (z = min)
    OutPlanes[4].min = {Box.min.x, Box.min.y, Box.min.z};
    OutPlanes[4].max = {Box.max.x, Box.max.y, Box.min.z};

    // back (z = max)
    OutPlanes[5].min = {Box.min.x, Box.min.y, Box.max.z};
    OutPlanes[5].max = {Box.max.x, Box.max.y, Box.max.z};

    return OutPlanes;
}
    void CScene::DrawBox(const BoundingBox & Box)
    {
        const Vector3 Size = PE::Math::BoxSize ( Box );
        const Vector3 Center = PE::Math::BoxCenter ( Box );
        DrawCubeWires( Center, Size.x, Size.y, Size.z, BLACK );
    }

    std::vector<SBall> CScene::GenerateBalls( int NumberOfBalls, const SBallGenerationParameters & BallGenerationParameters )
    {
        std::vector<SBall> OutBalls ( NumberOfBalls ); 
        for ( int i = 0; i < NumberOfBalls; i ++ ) 
        {
            OutBalls . push_back ( std::move ( GenerateBall ( BallGenerationParameters ) ) );
        }
        return OutBalls;
    }

    SBall CScene::GenerateBall( const SBallGenerationParameters &BallGenerationParameters )
    {
        // Location 
        std::uniform_real_distribution<float> UX(BallGenerationParameters . MinLocation.x, BallGenerationParameters . MaxLocation.x);
        std::uniform_real_distribution<float> UY(BallGenerationParameters . MinLocation.y, BallGenerationParameters . MaxLocation.y);
        std::uniform_real_distribution<float> UZ(BallGenerationParameters . MinLocation.z, BallGenerationParameters . MaxLocation.z);
        // Linear velocity
        std::uniform_real_distribution<float> ULVX(BallGenerationParameters . MinLinearVelocity.x, BallGenerationParameters . MaxLinearVelocity.x);
        std::uniform_real_distribution<float> ULVY(BallGenerationParameters . MinLinearVelocity.y, BallGenerationParameters . MaxLinearVelocity.y);
        std::uniform_real_distribution<float> ULVZ(BallGenerationParameters . MinLinearVelocity.z, BallGenerationParameters . MaxLinearVelocity.z);

        // Radius 
        std::uniform_real_distribution<float> URadius(BallGenerationParameters . MinRadius, BallGenerationParameters . MaxRadius );
        const float Radius = URadius(m_RandomGenerator);
        const float Mass = Radius * BallGenerationParameters . MassToRadius;
        const Color BallColor = PE::Math::ColorLerp ( GREEN, RED, ( Radius - BallGenerationParameters . MinRadius ) / ( BallGenerationParameters . MaxRadius - BallGenerationParameters . MinRadius ) );
        return SBall {  .Center = { UX(m_RandomGenerator), UY(m_RandomGenerator), UZ(m_RandomGenerator) }, 
                        .Radius = Radius ,
                        .Rotation = QuaternionIdentity(),
                        .LinearVelocity = { ULVX( m_RandomGenerator ), ULVY( m_RandomGenerator ), ULVZ( m_RandomGenerator ) },
                        .AngularVelocity = { 0, 0, 0 },
                        .Mass = Mass,
                        .Color = BallColor
                     };
    }
} // namespace PE
