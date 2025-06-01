#include "raylib.h"
#include "particle.h"
#include "sph.h"
#include "rlgl.h"
#include "rigidbody.h"

int main()
{
    InitWindow(600, 600, "SPH trial");
    SetTargetFPS(60);
    InitParticles();
    RigidBody rb;
    bool rbIn = false;
    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_R) && !rbIn)
        {
            InitRigidBody(&rb, (Vector2){300,300},100,50,30);
            rbIn = true;
        }
        if(rbIn)
        {
            ApplyBuoyancy(&rb);
            UpdateRigidBody(&rb, DT);
        }
        Update();
        BeginDrawing();
        ClearBackground(BLACK);
        if(rbIn)
        {
            DrawRigidBody(&rb);
        }
        DrawParticles();
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}