#pragma once
#include "raylib.h"

#define MAX_PARTICLES 500

typedef struct 
{
    Vector2 pos;
    Vector2 vel;
    Vector2 force;
    float rho;
    float p;
} Particle;

extern Particle particles[MAX_PARTICLES];

void InitParticles();
void DrawParticles();