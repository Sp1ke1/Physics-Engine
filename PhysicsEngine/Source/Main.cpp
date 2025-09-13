#include "raylib.h"
#include <Physics/Scene.hpp>

int main(void)
{
    CScene Scene{ SWindowParameters{}, SCameraParameters{} };
    while (!WindowShouldClose() )  
    {
        Scene . Update( GetFrameTime() );
        BeginDrawing();
            Scene . Render(); 
        EndDrawing();
        
    }
    CloseWindow();
    return 0;
}