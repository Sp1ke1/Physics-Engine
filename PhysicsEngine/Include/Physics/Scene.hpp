#pragma once
#include "raylib.h"
#include <string>

struct SWindowParameters
{   
    const int ScreenWidth = 800; 
    const int ScreenHeight = 600;
    const int TargetFPS = 120;
    const std::string Title = "Physics Engine";
};

struct SCameraParameters
{
    Vector3 Position = { 0.0f, 15.0f, 10.0f };
    Vector3 Target = { 0.0f, 0.0f, 0.0f };      
    Vector3 Up = { 0.0f, 1.0f, 0.0f };        
    float FovY = 45.0f;
    int Projection = CAMERA_PERSPECTIVE;
};

class CScene
{

public:
    CScene( const SWindowParameters& WindowParameters, const SCameraParameters& CameraParameters ); 
    void SetWindowParameters ( const SWindowParameters& WindowParameters );
    void SetCameraParameters ( const SCameraParameters& CameraParameters );
    void Update(float DeltaTime );
    void Render();


protected: 
    Camera3D m_Camera; 
};