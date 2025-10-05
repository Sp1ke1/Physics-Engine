#pragma once 
#include "raylib.h"
#include <string> 


namespace PE 
{
struct SWindowParameters
    {   
        int ScreenWidth = 800; 
        int ScreenHeight = 600;
        int TargetFPS = 120;
        std::string Title = "Physics Engine";
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
        Vector3 MinLinearVelocity = { -2.5f, -2.5f, -2.5f };
        Vector3 MaxLinearVelocity = { 2.5f, 2.5f, 2.5f };
        Vector3 MinAngularVelocity = { -2.0f, -2.0f , -2.0f };
        Vector3 MaxAngularVelocity = { 2.0f, 2.0f , 2.0f };
        float MinRadius = 0.5f; 
        float MaxRadius = 1.f; 
        float MassToRadius = 10.f; // Mass = Radius * MassToRadius 
    };
    
    struct SSimulationParameters 
    {
        int SimulationFrequency = 120; 
        int NumberOfSteps = 8;
        int NumberOfBalls = 30;
        int RandomSeed = 1337;
        float Slop = 0.0005f;
        float Gravity = 9.81f;
        float BallsRestitution = 0.3f;
        float BallFriction = 0.2f;
        float LinearDamping = 0.1f;
        float AngularDamping = 0.2f;
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