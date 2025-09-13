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
        UpdateSimulation( DeltaTime );
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
    }
    void CScene::Initialize( const SSceneParameters & SceneParameters )
    {
        InitWindow ( SceneParameters . WindowParameters . ScreenWidth, SceneParameters . WindowParameters . ScreenHeight, SceneParameters . WindowParameters . Title . c_str());
        SetTargetFPS ( SceneParameters . WindowParameters . TargetFPS );
        SetCameraParameters ( SceneParameters . CameraParameters );
        SetSimulationParameters ( SceneParameters . SimulationParameters);
    }

    void CScene::SetSimulationParameters ( const SSimulationParameters &SimulationParameters)
    {
        m_RandomGenerator = std::mt19937 ( SimulationParameters . RandomSeed );
        m_Balls = GenerateBalls ( SimulationParameters . NumberOfBalls, SimulationParameters . BallGenerationParameters );
        m_Gravity = SimulationParameters . Gravity;
        m_WorldBox = { .min = SimulationParameters . WorldBoxMin, .max = SimulationParameters . WorldBoxMax };
        m_WorldPlanes = BoundingBoxToPlanes ( m_WorldBox );
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
    void CScene::UpdateSimulation(float DeltaTime)
    {
        for ( auto & Ball : m_Balls ) 
        {
            Ball . LinearVelocity = Vector3Add ( Ball . LinearVelocity, Vector3Scale ( { 0.f, -m_Gravity, 0.f } , DeltaTime ) );
            Ball . Center = Vector3Add ( Ball . Center , Vector3Scale ( Ball . LinearVelocity, DeltaTime ) );
            for ( const auto & Plane : m_WorldPlanes )
            {
                const PE::Collision::SHitResult Hit = PE::Collision::TestSphereBox ( Ball . Center, Ball . Radius, Plane );
                if ( Hit . IsHit ) 
                {
                    std::cout << "Collided with plane!" << std::endl; 
                }   
            }
        }
    }
    std::array<BoundingBox, 6> CScene::BoundingBoxToPlanes(const BoundingBox &Box) const
    {

        std::array<BoundingBox,6> OutPlanes {}; 
        // Order: left, right, bottom, top, front, back

        // left  (x = min)
        OutPlanes[0].min = { Box.min.x, Box.min.y, Box.min.z };
        OutPlanes[0].max = { Box.min.x, Box.max.y, Box.max.z };

        // right (x = max)
        OutPlanes[1].min = { Box.max.x, Box.min.y, Box.min.z };
        OutPlanes[1].max = { Box.max.x, Box.max.y, Box.max.z };

        // bottom (y = min)
        OutPlanes[2].min = { Box.min.x, Box.min.y, Box.min.z };
        OutPlanes[2].max = { Box.max.x, Box.min.y, Box.max.z };

        // top (y = max)
        OutPlanes[3].min = { Box.min.x, Box.max.y, Box.min.z };
        OutPlanes[3].max = { Box.max.x, Box.max.y, Box.max.z };

        // front (z = min)
        OutPlanes[4].min = { Box.min.x, Box.min.y, Box.min.z };
        OutPlanes[4].max = { Box.max.x, Box.max.y, Box.min.z };

        // back (z = max)
        OutPlanes[5].min = { Box.min.x, Box.min.y, Box.max.z };
        OutPlanes[5].max = { Box.max.x, Box.max.y, Box.max.z };

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
        std::uniform_real_distribution<float> UX(BallGenerationParameters . MinLocation.x, BallGenerationParameters . MaxLocation.x);
        std::uniform_real_distribution<float> UY(BallGenerationParameters . MinLocation.y, BallGenerationParameters . MaxLocation.y);
        std::uniform_real_distribution<float> UZ(BallGenerationParameters . MinLocation.z, BallGenerationParameters . MaxLocation.z);
        std::uniform_real_distribution<float> URadius(BallGenerationParameters . MinRadius, BallGenerationParameters . MaxRadius );
        std::uniform_real_distribution<float> UMass(BallGenerationParameters . MinMass, BallGenerationParameters . MaxMass );

        return SBall {  .Center = { UX(m_RandomGenerator), UY(m_RandomGenerator), UZ(m_RandomGenerator) }, 
                        .Radius = URadius(m_RandomGenerator) ,
                        .Rotation = QuaternionIdentity(),
                        .LinearVelocity = { 0, 0, 0 },
                        .AngularVelocity = { 0, 0, 0 },
                        .Mass = UMass(m_RandomGenerator),
                        .Color = RED 
                     };
    }
} // namespace PE
