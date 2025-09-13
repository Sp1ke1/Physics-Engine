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
        int NumberOfBalls = 1;
        int RandomSeed = 42; 
        float Gravity = 9.81f;
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