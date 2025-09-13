
#include "Physics/Scene.hpp"
#include "raylib.h"

CScene::CScene()
{
    InitWindow(m_ScreenWidth, m_ScreenHeight, "Physics Engine");
    SetTargetFPS(m_TargetFPS);  
}


void CScene::Update( float DeltaTime )
{
    
}

void CScene::Render()
{
    ClearBackground(RAYWHITE);

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;     
     Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    BeginMode3D(camera);
        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawGrid(10, 1.0f);
    EndMode3D();
    DrawText(TextFormat("Physics Engine"), 20, 20, 20, BLACK);
}