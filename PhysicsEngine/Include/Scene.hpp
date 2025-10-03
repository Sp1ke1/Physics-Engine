#pragma once
#include "raylib.h"
#include "Object.hpp"
#include "Parameters.hpp"
#include "PhysicsBody.hpp"
#include <random> 
#include <vector>
#include <array>


namespace PE
{
    class CScene
    {
        
        public:
        CScene( const SSceneParameters & SceneParameters ); 
        void SetWindowParameters ( const SWindowParameters& WindowParameters );
        void SetCameraParameters ( const SCameraParameters& CameraParameters );
        void SetSimulationParameters ( const SSimulationParameters & SimulationParameters  );
        void Update(float DeltaTime );
        void Draw();
        void ClearSimulation();
        void RestartSimulation(); 
        
        
        protected: 
        
        
        std::vector<SPhysicsBody> GenerateBalls ( int NumberOfBalls, const SBallGenerationParameters & BallGenerationParameters );
        SPhysicsBody GenerateBall ( const SBallGenerationParameters & BallGenerationParameters );
        void GenerateObjects(); 
        void Initialize( const SSceneParameters & SceneParameters  ); 
        void DrawUI ();
        void DrawObject ( const SSimulationObject & Ball );
        void DrawBox ( const BoundingBox & Box, const Color & Color );
        void DrawBall ( const Vector3 & Location, float Radius, const Quaternion & Rotation, const Color & Color );
        // void DrawBallsDebugInfo();
        void SimulationStep ( float DeltaTime );
        void IntegrateForces ( float DeltaTime );
        void ResolveCollisions( float DeltaTime);
        void ResolveCollisionsPair ( SPhysicsBody & BodyA, SPhysicsBody & BodyB, float DeltaTime ); 
        
        Camera3D m_Camera;
        std::vector<SSimulationObject> m_Objects;
        std::vector<SPhysicsBody> m_PhysicsBodies;
        BoundingBox m_WorldBox;
        SSceneParameters m_SceneParameters; 
        
        private:
        std::array<SPhysicsBody, 6> BoundingBoxToPlanes ( const BoundingBox & Box ) const;
        std::mt19937 m_RandomGenerator;
        float m_TimeAccumulator = 0.f;
        float m_FixedDeltaTime = 0.f;
        double m_SimulationStartTime = 0.f;
        int m_NumberOfBalls = 0;
        bool m_IsPaused = false; 
    };
} // namespace PE