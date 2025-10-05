#pragma once
#include "raylib.h"

namespace PE 
{
    /**
     * @brief Lightweight renderable object that references a physics body by index.
     *
     * Stores the index into the scene's body array and a color for drawing.
     */
    struct SSimulationObject 
    {
        int PhysicsBodyIndex = 0; 
        Color Color;
    };
} // namespace PE