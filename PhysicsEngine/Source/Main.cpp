#include "raylib.h"
#include <Physics/Scene.hpp>

int main(void)
{
    CScene Scene { SSceneParameters {} };
    while (!WindowShouldClose() )  
    {
        Scene . Update( GetFrameTime() );
        BeginDrawing();
            Scene . Draw(); 
        EndDrawing();
        
    }
    CloseWindow();
    return 0;
}