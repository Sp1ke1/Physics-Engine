#pragma once 
#include "raylib.h"
#include <string> 


namespace PE 
{
struct SWindowParameters
    {   
        const int ScreenWidth = 800; 
        const int ScreenHeight = 600;
        const int TargetFPS = 120;
        const std::string Title = "Physics Engine";
    };
    
    struct SCameraParameters
    {
        Vector3 Position = { 0.0f, -5.f, 25.0f };
        Vector3 Target = { 0.0f, 0.0f, 0.0f };      
        Vector3 Up = { 0.0f, 1.0f, 0.0f };        
        float FovY = 45.0f;
        int Projection = CAMERA_PERSPECTIVE;
    };
    
    struct SBallGenerationParameters 
    {
        Vector3 MinLocation = { -5.0f, 5.0f, -5.0f };
        Vector3 MaxLocation = { 5.0f, 5.0f, 5.0f };
        Vector3 MinLinearVelocity = { -1.0f, -1.0f, -1.0f };
        Vector3 MaxLinearVelocity = { 1.0f, 1.0f, 1.0f };
        Vector3 MinAngularVelocity = { -1.0f, -1.0f , -1.0f };
        Vector3 MaxAngularVelocity = { 1.0f, 1.0f , 1.0f };
        float MinRadius = 0.5f; 
        float MaxRadius = 1.f; 
        float MassToRadius = 1.f; // Mass = Radius * MassToRadius 
    };
    
    struct SSimulationParameters 
    {
        int SimulationFrequency = 240; 
        int NumberOfSteps = 8;
        int NumberOfBalls = 25;
        int RandomSeed = 42;
        float Slop = 1e-5f;
        float Gravity = 9.81f;
        float BallsRestitution = 0.5f;
        Vector3 WorldBoxMin = { -7.5f, -7.5f, -7.5f }; 
        Vector3 WorldBoxMax = { 7.5f, 7.5f, 7.5f };
        SBallGenerationParameters BallGenerationParameters;
    };
    
    
    struct SSceneParameters 
    {
        SWindowParameters WindowParameters; 
        SCameraParameters CameraParameters; 
        SSimulationParameters SimulationParameters;
    };
} // namespace PE