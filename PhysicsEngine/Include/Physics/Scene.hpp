#pragma once
#include "raylib.h"
#include "Physics/Ball.hpp"
#include "Physics/Box.hpp"
#include "Physics/Parameters.hpp"
#include <random> 
#include <vector> 


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
        void DrawBox ( const SBox& Box );
        void DrawBalls ();
        void DrawWorld (); 
        void UpdateSimulation ( float DeltaTime ); 
        
        Camera3D m_Camera;
        std::vector<SBall> m_Balls; 
        SBox m_WorldBox;
        float m_Gravity; 
        
        private: 
        std::mt19937 m_RandomGenerator; 
    };
} // namespace PE