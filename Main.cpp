#include "raylib.h"
#include <Scene.hpp>

int main(void)
{
    PE::CScene Scene { PE::SSceneParameters {} };
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