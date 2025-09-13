#pragma once
#include "Physics/Scene.hpp"
#include "raymath.h"

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
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    BeginMode3D(m_Camera);
        DrawBalls(); 
        DrawGrid(10, 1.0f);
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
}

void CScene::DrawBall(const SBall &Ball)
{   
    DrawSphere( Ball . Location, Ball . Radius, Ball . Color );
    DrawSphereWires( Ball . Location, Ball . Radius, 8, 8, BLACK);
}

void CScene::DrawBalls()
{   
    for ( const auto & Ball : m_Balls ) 
    {
        DrawBall ( Ball ); 
    }
}

void CScene::UpdateSimulation(float DeltaTime)
{
    for ( auto & Ball : m_Balls ) 
    {
        Ball . LinearVelocity = Vector3Add ( Ball . LinearVelocity, Vector3Scale ( { 0.f, -m_Gravity, 0.f } , DeltaTime ) );
        Ball . Location = Vector3Add ( Ball . Location, Vector3Scale ( Ball . LinearVelocity, DeltaTime ) );
    }
}
void CScene::DrawBox(const SBox & Box)
{
    const Vector3 Size = Box . Size(); 
    const Vector3 Center = Box . Center(); 
    DrawCubeWires( Center, Size.x, Size.y, Size.z, Box . Color );
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
    
    return SBall { .Location = { UX(m_RandomGenerator), UY(m_RandomGenerator), UZ(m_RandomGenerator) },
                   .Rotation = QuaternionIdentity(),
                   .LinearVelocity = { 0, 0, 0 },
                   .AngularVelocity = { 0, 0, 0 },
                   .Radius = URadius(m_RandomGenerator),
                   .Mass = UMass(m_RandomGenerator),
                   .Color = RED 
                 };
}
