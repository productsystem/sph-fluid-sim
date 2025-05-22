#include "particle.h"
#include "constants.h"
#include <stdlib.h>
#include <time.h>

Particle particles[MAX_PARTICLES];

void InitParticles()
{
    int i = 0;
    float spacing = H;
    float startX = GetScreenWidth() / 4.0f;
    float startY = EPS;

    srand(time(NULL));

    for (float y = startY; y < GetScreenHeight() - EPS * 2.0f; y += spacing)
    {
        for (float x = startX; x <= GetScreenWidth() / 2.0f; x += spacing)
        {
            if (i >= MAX_PARTICLES)
                return;
            float jitter = (float)rand() / (float)RAND_MAX;
            float jitterX = jitter * spacing * 0.1f;
            float jitterY = jitter * spacing * 0.1f;

            particles[i].pos.x = x + jitterX;
            particles[i].pos.y = y + jitterY;

            particles[i].vel = (Vector2){0.f};
            particles[i].force = (Vector2){0.f};
            particles[i].rho = 0.0f;
            particles[i].p = 0.0f;
            i++;
        }
    }
}


void DrawParticles()
{
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        DrawCircleV(particles[i].pos, 2, BLUE);
    }
}