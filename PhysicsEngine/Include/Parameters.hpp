#pragma once 
#include "raylib.h"
#include <string> 


namespace PE 
{
/**
 * @brief Window configuration parameters.
 */
struct SWindowParameters
    {   
        int ScreenWidth = 1920; 
        int ScreenHeight = 1080;
        int TargetFPS = 120;
        std::string Title = "Physics Engine";
    };
    
/**
 * @brief Camera parameters used for the 3D view.
 */
    struct SCameraParameters
    {
        Vector3 Position = { 0.0f, -5.f, 25.0f };
        Vector3 Target = { 0.0f, 0.0f, 0.0f };      
        Vector3 Up = { 0.0f, 1.0f, 0.0f };        
        float FovY = 45.0f;
        int Projection = CAMERA_PERSPECTIVE;
    };
    
/**
 * @brief Parameters used to randomize generated balls.
 */
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
    
/**
 * @brief Parameters that configure the simulation behaviour.
 */
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
    
    
/**
 * @brief Top-level scene parameters grouping window, camera and sim settings.
 */
    struct SSceneParameters 
    {
        SWindowParameters WindowParameters; 
        SCameraParameters CameraParameters; 
        SSimulationParameters SimulationParameters;
    };
} // namespace PE