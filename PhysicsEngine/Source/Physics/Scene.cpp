#include "raylib.h"
#include "Physics/Scene.hpp"

CScene::CScene( const SWindowParameters& WindowParameters, const SCameraParameters& CameraParameters )
{
    InitWindow (WindowParameters.ScreenWidth, WindowParameters.ScreenHeight, WindowParameters.Title.c_str());
    SetTargetFPS ( WindowParameters.TargetFPS );
    SetCameraParameters ( CameraParameters );
}

void CScene::Update(float DeltaTime) 
{
    
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

void CScene::Render()
{
    ClearBackground(RAYWHITE);
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    BeginMode3D(m_Camera);
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawGrid(10, 1.0f);
    EndMode3D();
    DrawText(TextFormat("Physics Engine"), 20, 20, 20, BLACK);
}