#pragma once
#include "raylib.h"
#include "Ball.hpp"
#include "Parameters.hpp"
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
        std::vector<SBall> GenerateBalls( int NumberOfBalls, const SBallGenerationParameters & BallGenerationParameters );
        SBall GenerateBall ( const SBallGenerationParameters & BallGenerationParameters );
        
        
        protected: 
        
        
        void Initialize( const SSceneParameters & SceneParameters  ); 
        void DrawBall ( const SBall& Ball );
        void DrawBox ( const BoundingBox & Box );
        void DrawBalls ();
        void DrawWorld (); 
        void UpdateSimulation ( float DeltaTime );
        void IntegrateLinear( float DeltaTime );
        void ResolveCollisions( float DeltaTime);
        void ResolveCollisionsWithWalls ( float DeltaTime );
        void ResolveCollisionsBetweenBalls ( float DeltaTime );
        
        Camera3D m_Camera;
        std::vector<SBall> m_Balls; 
        BoundingBox m_WorldBox;
        float m_Gravity; 
        float m_BallsRestitution;
        
        private:
        std::array<BoundingBox, 6> BoundingBoxToPlanes ( const BoundingBox & Box ) const;
        std::array<BoundingBox, 6> m_WorldPlanes; 
        std::mt19937 m_RandomGenerator; 
    };
} // namespace PE