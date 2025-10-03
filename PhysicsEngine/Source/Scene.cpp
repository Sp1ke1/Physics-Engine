#pragma once
#include "Scene.hpp"
#include "Collision.hpp"
#include "raymath.h"
#include "Math.hpp"
#include <iostream>
#include <cmath>

namespace PE 
{
    CScene::CScene( const SSceneParameters & SceneParameters )
    {
        Initialize( SceneParameters );
    }

    void CScene::Update(float DeltaTime) 
    {
        UpdateCamera ( &m_Camera, CAMERA_FREE );
        
        if ( IsKeyPressed( KEY_R ) ) 
        {
            RestartSimulation();
        }

        if ( IsKeyPressed( KEY_ENTER ) ) 
        {
            m_IsPaused = !m_IsPaused;
        }

        if ( m_IsPaused ) 
        {
            return; 
        }

        m_TimeAccumulator += DeltaTime;
        while ( m_TimeAccumulator >= m_FixedDeltaTime )
        {
            SimulationStep( m_FixedDeltaTime );
            m_TimeAccumulator -= m_FixedDeltaTime;
        }
    }
    void CScene::SetWindowParameters(const SWindowParameters &WindowParameters)
    {
        SetWindowSize ( WindowParameters.ScreenWidth, WindowParameters . ScreenHeight );
        SetTargetFPS ( WindowParameters . TargetFPS );
    }
    void CScene::SetCameraParameters(const SCameraParameters &CameraParameters)
    {
        m_Camera . position = CameraParameters . Position;
        m_Camera . target = CameraParameters . Target;
        m_Camera . up = CameraParameters . Up;
        m_Camera . fovy = CameraParameters . FovY;
        m_Camera . projection = CameraParameters . Projection;
    }

    void CScene::Draw()
    {
        ClearBackground(RAYWHITE);
        BeginMode3D(m_Camera);
            for ( const auto & Object : m_Objects )
            {
                DrawObject ( Object );
            }
        EndMode3D();
        DrawUI(); 
    }
    void CScene::ClearSimulation()
    {
        m_Objects . clear();
        m_PhysicsBodies . clear();
        m_TimeAccumulator = 0.f;
        m_NumberOfBalls = 0; 
        m_SimulationStartTime = GetTime();
    }
    void CScene::Initialize(const SSceneParameters & SceneParameters)
    {
        m_SceneParameters = SceneParameters;
        InitWindow ( m_SceneParameters . WindowParameters . ScreenWidth, m_SceneParameters . WindowParameters . ScreenHeight, SceneParameters . WindowParameters . Title . c_str());
        SetTargetFPS ( m_SceneParameters . WindowParameters . TargetFPS );
        SetCameraParameters ( m_SceneParameters . CameraParameters );
        SetSimulationParameters ( m_SceneParameters . SimulationParameters);
        m_SimulationStartTime = GetTime(); 
        DisableCursor();
        RestartSimulation();
    }

    void CScene::DrawUI()
    {
        const double ElapsedTime = GetTime() - m_SimulationStartTime; 
        char Buffer [64];
        snprintf(Buffer, sizeof(Buffer), "Time: %.2f s", ElapsedTime );
        DrawText(Buffer, 0, 0, 20, BLACK);
        snprintf(Buffer, sizeof(Buffer), "Number of balls: %d", m_NumberOfBalls );
        DrawText(Buffer, 0, 20, 20, BLACK);
        snprintf(Buffer, sizeof ( Buffer ), "Frame time: %.3f ms", GetFrameTime() * 1000.f );
        DrawText(Buffer, 0, 40, 20, BLACK);
        if ( m_SceneParameters.SimulationParameters.PrintBallsDebugInfo )
        {
            // DrawBallsDebugInfo();
        }
    }

    void CScene::DrawObject(const SSimulationObject & Object )
    {
        const auto & Body = m_PhysicsBodies [ Object . PhysicsBodyIndex ];
        switch ( Body. Shape . Type )
        {
            case EShapeType::Sphere:
            {
                DrawBall ( Body . Position, 
                           Body . Shape . Sphere . Radius, 
                           Body . Rotation, 
                           Object . Color );
                break;
            }

            case EShapeType::Box:
            {
                DrawBox ( { .min = Vector3Subtract ( Body . Position, Body . Shape . Box . HalfSize ),
                            .max = Vector3Add ( Body . Position, Body . Shape . Box . HalfSize ) }, 
                          Object . Color );
                break;
            }
        }
    }

    void CScene::SetSimulationParameters ( const SSimulationParameters &SimulationParameters)
    {
        m_RandomGenerator = std::mt19937 ( SimulationParameters . RandomSeed );
        m_WorldBox = { .min = SimulationParameters . WorldBoxMin, .max = SimulationParameters . WorldBoxMax };
        m_FixedDeltaTime = 1.f / static_cast<float> ( SimulationParameters . SimulationFrequency );
    }

    void CScene::DrawBall( const Vector3 & Location, float Radius, const Quaternion & Rotation, const Color & Color )
    {   
        DrawSphere ( Location, Radius, Color );
        Vector3 localX = { Radius, 0.f, 0.f };
        Vector3 localY = { 0.f, Radius, 0.f };
        Vector3 localZ = { 0.f, 0.f, Radius };

        Vector3 worldX = Vector3Add(Location, Vector3RotateByQuaternion(localX, Rotation));
        Vector3 worldY = Vector3Add(Location, Vector3RotateByQuaternion(localY, Rotation));
        Vector3 worldZ = Vector3Add(Location, Vector3RotateByQuaternion(localZ, Rotation));

        /*
        DrawLine3D(Location, worldX, Color);
        DrawLine3D(Location, worldY, GREEN);
        DrawLine3D(Location, worldZ, BLUE);

        const float markerSize = Radius * 0.12f;
        DrawCube(worldX, markerSize, markerSize, markerSize, RED);
        DrawCube(worldY, markerSize, markerSize, markerSize, GREEN);
        DrawCube(worldZ, markerSize, markerSize, markerSize, BLUE);

        
        const int segments = 36;
        const float twoPi = 6.28318530718f;
        Vector3 prev = { Radius, 0.f, 0.f };
        prev = Vector3Add(Location, Vector3RotateByQuaternion(prev, Rotation));
        for (int s = 1; s <= segments; ++s)
        {
            float t = (float)s / (float)segments;
            float ang = t * twoPi;
            Vector3 localP = { Radius * cosf(ang), 0.f, Radius * sinf(ang) };
            Vector3 worldP = Vector3Add(Location, Vector3RotateByQuaternion(localP, Rotation));
            DrawLine3D(prev, worldP, DARKGRAY);
            prev = worldP;
        }
        */
    }

    /* 
    void CScene::DrawBallsDebugInfo()
    {
        char Buffer [64];
        const int BallsPrintLocationBaseOffset = 60; 
        const int BallsPrintLocationLineOffset = 20;
        for ( auto i = 0; i < m_Balls . size(); ++ i )
        {
            snprintf ( Buffer, sizeof ( Buffer ), "[%d] Pos: (%.2f, %.2f, %.2f)", 
            i, 
            m_Balls[i] . Center . x, 
            m_Balls[i] . Center . y, 
            m_Balls[i] . Center . z 
            );
            DrawText( Buffer, 0, BallsPrintLocationBaseOffset + BallsPrintLocationLineOffset * i, 20, BLACK);

            snprintf ( Buffer, sizeof ( Buffer ), "[%d] Lin Vel: (%.2f, %.2f, %.2f)", 
            i, 
            m_Balls[i] . LinearVelocity . x, 
            m_Balls[i] . LinearVelocity . y, 
            m_Balls[i] . LinearVelocity . z 
            );
            DrawText( Buffer, 0, BallsPrintLocationBaseOffset + BallsPrintLocationLineOffset * i + BallsPrintLocationLineOffset , 20, BLACK);

            snprintf ( Buffer, sizeof ( Buffer ), "[%d] Ang Vel: (%.2f, %.2f, %.2f)", 
            i, 
            m_Balls[i] . AngularVelocity . x, 
            m_Balls[i] . AngularVelocity . y, 
            m_Balls[i] . AngularVelocity . z 
            );
            DrawText( Buffer, 0, BallsPrintLocationBaseOffset + BallsPrintLocationLineOffset * i + BallsPrintLocationLineOffset * 2, 20, BLACK);
        } 
    }
*/
    void CScene::SimulationStep (float DeltaTime)
    {
        IntegrateForces( DeltaTime );
        ResolveCollisions ( DeltaTime );
    }
    void CScene::IntegrateForces( float DeltaTime )
    {
        for (auto & PhysicsBody : m_PhysicsBodies )
        {
            if ( PhysicsBody . IsStatic )
            {
                continue; 
            }
            // Apply gravity force
            PhysicsBody.LinearVelocity = Vector3Add(PhysicsBody.LinearVelocity,
                Vector3Scale({0.f, -m_SceneParameters.SimulationParameters.Gravity, 0.f}, DeltaTime));

            // Apply linear damping (exponential decay) so velocity reduces over time
            if (PhysicsBody.LinearDamping  > 0.f)
            {
                const float LinearDampingFactor = expf(-PhysicsBody.LinearDamping * DeltaTime);
                PhysicsBody.LinearVelocity = Vector3Scale(PhysicsBody.LinearVelocity, LinearDampingFactor);
            }
            // Update position from linear velocity
            PhysicsBody.Position = Vector3Add(PhysicsBody.Position, Vector3Scale(PhysicsBody.LinearVelocity, DeltaTime));
            
            // Update rotation quaternion from angular velocity (axis-angle)
            const float Omega = Vector3Length(PhysicsBody.AngularVelocity);
            if (Omega > 1e-8f)
            {
                const Vector3 Axis = Vector3Scale(PhysicsBody.AngularVelocity, 1.0f / Omega);
                const float Angle = Omega * DeltaTime;
                const Quaternion DeltaRotation = QuaternionFromAxisAngle(Axis, Angle);
                PhysicsBody.Rotation = QuaternionMultiply(DeltaRotation, PhysicsBody.Rotation);
                PhysicsBody.Rotation = QuaternionNormalize(PhysicsBody.Rotation);
            }

            // Apply angular damping (exponential decay) so spin reduces over time
            if (PhysicsBody.AngularDamping > 0.f)
            {
                const float Factor = expf(-PhysicsBody.AngularDamping * DeltaTime);
                PhysicsBody.AngularVelocity = Vector3Scale(PhysicsBody.AngularVelocity, Factor);
            }
        }
    }
        
    void CScene::ResolveCollisions(float DeltaTime)
    {
        for ( size_t i = 0; i < m_SceneParameters . SimulationParameters . NumberOfSteps; i++ )
        {
            for ( size_t j = 0; j < m_PhysicsBodies.size(); j++ ) 
            {
                for ( size_t k = j + 1; k < m_PhysicsBodies.size(); k++ ) 
                {
                    ResolveCollisionsPair ( m_PhysicsBodies [ j ], m_PhysicsBodies [ k ], DeltaTime ); 
                }
            }
        }
    }

    void CScene::ResolveCollisionsPair(SPhysicsBody &BodyA, SPhysicsBody &BodyB, float DeltaTime)
    {
        if ( BodyA.IsStatic && BodyB.IsStatic ) 
        {
            return; 
        }

        const PE::Collision::SHitResult Hit = PE::Collision::TestCollision ( BodyA, BodyB );
        if (Hit.IsHit)
        {
            std::cout << "Hit detected!" << "Id a: " << BodyA.Id << " Id b: " << BodyB.Id << " Time: " << GetTime() - m_SimulationStartTime << std::endl;
            // Positional correction
            const float Penetration = Hit.Penetration - m_SceneParameters.SimulationParameters.Slop;
            const float SumInvMass = BodyA.InvMass + BodyB.InvMass;
            if ( Penetration > 0.f ) 
            {
                if ( SumInvMass > 0.f ) 
                {
                    const Vector3 Correction = Vector3Scale(Hit.Normal, Penetration / SumInvMass);
                    if (!BodyA.IsStatic)
                    {
                        BodyA.Position = Vector3Add(BodyA.Position, Vector3Scale(Correction, BodyA.InvMass));
                    }
                    if (!BodyB.IsStatic)
                    {
                        BodyB.Position = Vector3Subtract(BodyB.Position, Vector3Scale(Correction, BodyB.InvMass));
                    }
                }
            }
            // Velocity change (impulse)
            const Vector3 RelativeVelocity = Vector3Subtract( BodyA . LinearVelocity, BodyB . LinearVelocity);
            const float RelativeNormalVelocity = Vector3DotProduct(RelativeVelocity, Hit.Normal);
            if ( RelativeNormalVelocity < 0.f) 
            {
                // linear impulse (instant velocity change)
                const float CoefficientOfRestitution = std::fmin(BodyA.Restitution, BodyB.Restitution);
                const float NormalImpulseMagnitude = (1.f + CoefficientOfRestitution) * (-RelativeNormalVelocity) / SumInvMass;
                const Vector3 NormalImpulse = Vector3Scale(Hit.Normal, NormalImpulseMagnitude);

                if ( ! BodyA.IsStatic )
                {
                    BodyA.LinearVelocity = Vector3Add(BodyA.LinearVelocity, Vector3Scale(NormalImpulse, BodyA.InvMass));
                }
                if ( ! BodyB.IsStatic )
                {
                    BodyB.LinearVelocity = Vector3Subtract(BodyB.LinearVelocity, Vector3Scale(NormalImpulse, BodyB.InvMass));
                }
                /* 
                // angular impulse (instant angular velocity change)
                const float MomentOfInertiaA = BodyA.Shape.GetMomentOfInertia( (BodyA.IsStatic) ? 0.f : BodyA.Mass );
                const float MomentOfInertiaB = BodyB.Shape.GetMomentOfInertia( (BodyB.IsStatic) ? 0.f : BodyB.Mass );
                const float InverseMomentOfIntertiaA = (MomentOfInertiaA > 0.f && !BodyA.IsStatic) ? (1.f / MomentOfInertiaA) : 0.f;
                const float InverseMomentOfIntertiaB = (MomentOfInertiaB > 0.f && !BodyB.IsStatic) ? (1.f / MomentOfInertiaB) : 0.f;
                const Vector3 RelativeContactPositionBodyA = Vector3Subtract(Hit.ContactPoint, BodyA.Position);
                const Vector3 RelativeContactPositionBodyB = Vector3Subtract(Hit.ContactPoint, BodyB.Position);            
                
                if ( ! BodyA.IsStatic ) 
                { 
                    BodyA.AngularVelocity = Vector3Add(BodyA.AngularVelocity, Vector3Scale(Vector3CrossProduct(RelativeContactPositionBodyA, NormalImpulse), InverseMomentOfIntertiaA)); 
                }
                if ( ! BodyB.IsStatic ) 
                { 
                    BodyB.AngularVelocity = Vector3Subtract(BodyB.AngularVelocity, Vector3Scale(Vector3CrossProduct(RelativeContactPositionBodyB, NormalImpulse), InverseMomentOfIntertiaB)); 
                }
                */
            }
        }   
    }
    std::array<SPhysicsBody, 6> CScene::BoundingBoxToPlanes(const BoundingBox &Box) const
    {
        // Create 6 thin static box bodies representing the world planes (left, right, bottom, top, front, back)
        std::array<SPhysicsBody, 6> OutPlanes{};
        const Vector3 min = Box.min;
        const Vector3 max = Box.max;

        // left (x = min)
        {
            Vector3 center = { min.x, 0.5f * (min.y + max.y), 0.5f * (min.z + max.z) };
            Vector3 half = { 0.f, 0.5f * (max.y - min.y), 0.5f * (max.z - min.z) };
            OutPlanes[0] = SPhysicsBody{};
            OutPlanes[0].Position = center;
            OutPlanes[0].Rotation = QuaternionIdentity();
            OutPlanes[0].Shape.Type = EShapeType::Box;
            OutPlanes[0].Shape.Box.HalfSize = half;
            OutPlanes[0].IsStatic = true;
            OutPlanes[0].Mass = 0.f; OutPlanes[0].InvMass = 0.f;
        }

        // right (x = max)
        {
            Vector3 center = { max.x, 0.5f * (min.y + max.y), 0.5f * (min.z + max.z) };
            Vector3 half = { 0.f, 0.5f * (max.y - min.y), 0.5f * (max.z - min.z) };
            OutPlanes[1] = SPhysicsBody{};
            OutPlanes[1].Position = center;
            OutPlanes[1].Rotation = QuaternionIdentity();
            OutPlanes[1].Shape.Type = EShapeType::Box;
            OutPlanes[1].Shape.Box.HalfSize = half;
            OutPlanes[1].IsStatic = true;
            OutPlanes[1].Mass = 0.f; OutPlanes[1].InvMass = 0.f;
        }

        // bottom (y = min)
        {
            Vector3 center = { 0.5f * (min.x + max.x), min.y, 0.5f * (min.z + max.z) };
            Vector3 half = { 0.5f * (max.x - min.x), 0.f, 0.5f * (max.z - min.z) };
            OutPlanes[2] = SPhysicsBody{};
            OutPlanes[2].Position = center;
            OutPlanes[2].Rotation = QuaternionIdentity();
            OutPlanes[2].Shape.Type = EShapeType::Box;
            OutPlanes[2].Shape.Box.HalfSize = half;
            OutPlanes[2].IsStatic = true;
            OutPlanes[2].Mass = 0.f; OutPlanes[2].InvMass = 0.f;
        }

        // top (y = max)
        {
            Vector3 center = { 0.5f * (min.x + max.x), max.y, 0.5f * (min.z + max.z) };
            Vector3 half = { 0.5f * (max.x - min.x), 0.f, 0.5f * (max.z - min.z) };
            OutPlanes[3] = SPhysicsBody{};
            OutPlanes[3].Position = center;
            OutPlanes[3].Rotation = QuaternionIdentity();
            OutPlanes[3].Shape.Type = EShapeType::Box;
            OutPlanes[3].Shape.Box.HalfSize = half;
            OutPlanes[3].IsStatic = true;
            OutPlanes[3].Mass = 0.f; OutPlanes[3].InvMass = 0.f;
        }

        // front (z = min)
        {
            Vector3 center = { 0.5f * (min.x + max.x), 0.5f * (min.y + max.y), min.z };
            Vector3 half = { 0.5f * (max.x - min.x), 0.5f * (max.y - min.y), 0.f };
            OutPlanes[4] = SPhysicsBody{};
            OutPlanes[4].Position = center;
            OutPlanes[4].Rotation = QuaternionIdentity();
            OutPlanes[4].Shape.Type = EShapeType::Box;
            OutPlanes[4].Shape.Box.HalfSize = half;
            OutPlanes[4].IsStatic = true;
            OutPlanes[4].Mass = 0.f; OutPlanes[4].InvMass = 0.f;
        }

        // back (z = max)
        {
            Vector3 center = { 0.5f * (min.x + max.x), 0.5f * (min.y + max.y), max.z };
            Vector3 half = { 0.5f * (max.x - min.x), 0.5f * (max.y - min.y), 0.f };
            OutPlanes[5] = SPhysicsBody{};
            OutPlanes[5].Position = center;
            OutPlanes[5].Rotation = QuaternionIdentity();
            OutPlanes[5].Shape.Type = EShapeType::Box;
            OutPlanes[5].Shape.Box.HalfSize = half;
            OutPlanes[5].IsStatic = true;
            OutPlanes[5].Mass = 0.f; OutPlanes[5].InvMass = 0.f;
        }
        return OutPlanes;
    }
    void CScene::DrawBox(const BoundingBox & Box, const Color & Color)
    {
        const Vector3 Size = PE::Math::BoxSize ( Box );
        const Vector3 Center = PE::Math::BoxCenter ( Box );
        DrawCubeWires( Center, Size.x, Size.y, Size.z, Color );
    }

    std::vector<SPhysicsBody> CScene::GenerateBalls( int NumberOfBalls, const SBallGenerationParameters & BallGenerationParameters )
    {
        std::vector<SPhysicsBody> OutBalls;
        OutBalls.reserve( NumberOfBalls ); 
        for ( int i = 0; i < NumberOfBalls; i ++ ) 
        {
            OutBalls . push_back ( std::move ( GenerateBall ( BallGenerationParameters ) ) );
        }
        return OutBalls;
    }

    SPhysicsBody CScene::GenerateBall( const SBallGenerationParameters &BallGenerationParameters )
    {
        // Location 
        std::uniform_real_distribution<float> UX(BallGenerationParameters . MinLocation.x, BallGenerationParameters . MaxLocation.x);
        std::uniform_real_distribution<float> UY(BallGenerationParameters . MinLocation.y, BallGenerationParameters . MaxLocation.y);
        std::uniform_real_distribution<float> UZ(BallGenerationParameters . MinLocation.z, BallGenerationParameters . MaxLocation.z);
        // Linear velocity
        std::uniform_real_distribution<float> ULVX(BallGenerationParameters . MinLinearVelocity.x, BallGenerationParameters . MaxLinearVelocity.x);
        std::uniform_real_distribution<float> ULVY(BallGenerationParameters . MinLinearVelocity.y, BallGenerationParameters . MaxLinearVelocity.y);
        std::uniform_real_distribution<float> ULVZ(BallGenerationParameters . MinLinearVelocity.z, BallGenerationParameters . MaxLinearVelocity.z);

        // Radius 
        std::uniform_real_distribution<float> URadius(BallGenerationParameters . MinRadius, BallGenerationParameters . MaxRadius );
        const float Radius = URadius(m_RandomGenerator);
        const float Mass = Radius * BallGenerationParameters . MassToRadius;
        const Color BallColor = PE::Math::ColorLerp ( GREEN, RED, ( Radius - BallGenerationParameters . MinRadius ) / ( BallGenerationParameters . MaxRadius - BallGenerationParameters . MinRadius ) );
        return SPhysicsBody { 
                        .Position = { UX(m_RandomGenerator), UY(m_RandomGenerator), UZ(m_RandomGenerator) }, 
                        .Rotation = QuaternionIdentity(),
                        .LinearVelocity = { ULVX( m_RandomGenerator ), ULVY( m_RandomGenerator ), ULVZ( m_RandomGenerator ) },
                        .AngularVelocity = { 0, 0, 0 },
                        .Shape = { .Type = EShapeType::Sphere, .Sphere = { .Radius = Radius } },
                        .Mass = Mass,
                        .InvMass = (Mass > 0.f) ? (1.f / Mass) : 0.f,
                        .IsStatic = false,
                        .Restitution = m_SceneParameters . SimulationParameters . BallsRestitution,
                        .Friction = m_SceneParameters . SimulationParameters . BallFriction,
                        .AngularDamping = m_SceneParameters . SimulationParameters . AngularDamping,
                        .LinearDamping = m_SceneParameters . SimulationParameters . LinearDamping,
                     };
    }
    
    void CScene::RestartSimulation()
    {
        ClearSimulation();
        GenerateObjects();
    }

    void CScene::GenerateObjects()
    {
        std::vector <SPhysicsBody> Balls = GenerateBalls ( m_SceneParameters . SimulationParameters . NumberOfBalls, m_SceneParameters . SimulationParameters . BallGenerationParameters );
        m_WorldBox = { .min = m_SceneParameters . SimulationParameters . WorldBoxMin, .max = m_SceneParameters .SimulationParameters . WorldBoxMax };
        std::array<SPhysicsBody, 6> WorldPlanes = BoundingBoxToPlanes ( m_WorldBox );
        m_NumberOfBalls = static_cast<int> ( Balls . size() );
        for ( auto && Ball : Balls ) 
        {
            const int Index = static_cast<int> ( m_PhysicsBodies.size() );
            Ball . Id = Index; 
            m_PhysicsBodies . push_back ( std::move ( Ball ) );
            SSimulationObject Object { 
                .PhysicsBodyIndex = Index,
                .Color = PE::Math::ColorLerp ( GREEN, RED, ( Ball.Shape.Sphere.Radius - m_SceneParameters . SimulationParameters . BallGenerationParameters . MinRadius ) / ( m_SceneParameters . SimulationParameters . BallGenerationParameters . MaxRadius - m_SceneParameters . SimulationParameters . BallGenerationParameters . MinRadius ) ),
            };
            m_Objects . push_back ( std::move ( Object ) );
        }
    
        for ( auto && Plane : WorldPlanes ) 
        {
            const int Index = static_cast<int> ( m_PhysicsBodies.size() );
            Plane . Id = Index; 
            m_PhysicsBodies . push_back ( std::move ( Plane ) );
            SSimulationObject Object { 
                .PhysicsBodyIndex = Index,
                .Color = GRAY,
            };
            m_Objects . push_back ( std::move ( Object ) );
        }
    }
} // namespace PE
