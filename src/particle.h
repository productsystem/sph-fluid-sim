#pragma once
#include "raylib.h"

#define MAX_PARTICLES 3000

#define HASH_TABLE_SIZE 8192
#define CELL_SIZE H
#define MAX_PARTICLES_PER_CELL 10
#define P1 73856093
#define P2 19349663

typedef struct 
{
    Vector2 pos;
    Vector2 vel;
    Vector2 force;
    float rho;
    float p;
} Particle;

typedef struct 
{
    Particle *particles[MAX_PARTICLES_PER_CELL];
    int count;
} Cell;

extern Cell hashTable[HASH_TABLE_SIZE];

extern Particle particles[MAX_PARTICLES];

int SpatialHash(Vector2 pos);
void ClearHashTable();
void InsertParticle(Particle *p);
void BuildHashTable();
void InitParticles();
void DrawParticles();