#include "raylib.h"
#include "particle.h"
#include "sph.h"
#include "rlgl.h"

int main()
{
    InitWindow(600, 600, "SPH trial");
    SetTargetFPS(60);
    InitParticles();
    while(!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawParticles();
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
}