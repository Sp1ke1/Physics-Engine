#pragma once
#include "raylib.h"
#include "Physics/Ball.hpp"
#include "Physics/Box.hpp"
#include <random> 
#include <string>
#include <vector> 

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

struct SBallGenerationParameters 
{
    Vector3 MinLocation = { -5.0f, 5.0f, -5.0f };
    Vector3 MaxLocation = { 5.0f, 5.0f, 5.0f };
    float MinRadius = 0.5f; 
    float MaxRadius = 2.0f; 
    float MinMass = 1.0f; 
    float MaxMass = 10.0f; 
};

struct SSimulationParameters 
{
    int NumberOfBalls = 10;
    int RandomSeed = 42; 
    float Gravity = 9.81f;
    SBallGenerationParameters BallGenerationParameters;
};


struct SSceneParameters 
{
    SWindowParameters WindowParameters; 
    SCameraParameters CameraParameters; 
    SSimulationParameters SimulationParameters;
};

class CScene
{

public:
    CScene( const SSceneParameters & SceneParameters ); 
    void SetWindowParameters ( const SWindowParameters& WindowParameters );
    void SetCameraParameters ( const SCameraParameters& CameraParameters );
    void SetSimulationParameters ( const SSimulationParameters & SimulationParameters  );
    void Update(float DeltaTime );
    void Draw();
    std::vector<SBall> GenerateBalls( int NumberOfBalls, const SBallGenerationParameters & BallGenerationParameters );
    SBall GenerateBall ( const SBallGenerationParameters & BallGenerationParameters );


protected: 


    void Initialize( const SSceneParameters & SceneParameters  ); 
    void DrawBall ( const SBall& Ball );
    void DrawBox ( const SBox& Box );
    void DrawBalls ();
    void UpdateSimulation ( float DeltaTime ); 

    Camera3D m_Camera;
    std::vector<SBall> m_Balls; 
    SBox m_WorldBox;
    float m_Gravity = 9.81f; 

private: 
    std::mt19937 m_RandomGenerator; 
};