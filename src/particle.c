#include "particle.h"
#include "constants.h"
#include <stdlib.h>
#include <time.h>

Particle particles[MAX_PARTICLES];

Cell hashTable[HASH_TABLE_SIZE];

void ClearHashTable()
{
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        hashTable[i].count = 0;
    }
}

int SpatialHash(Vector2 pos)
{
    int x = (int)floor(pos.x / CELL_SIZE);
    int y = (int)floor(pos.y / CELL_SIZE);
    int hash = ((x*P1) ^ (y*P2)) % HASH_TABLE_SIZE;
    if(hash <0)
    {
        return hash + HASH_TABLE_SIZE;
    }
    else
    {
        return hash;
    }
}

void InsertParticle(Particle *p)
{
    int hash = SpatialHash(p->pos);
    if(hashTable[hash].count < MAX_PARTICLES_PER_CELL)
    {
        hashTable[hash].particles[hashTable[hash].count++] = p;
    }
}

void BuildHashTable()
{
    ClearHashTable();
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        InsertParticle(&particles[i]);
    }
}

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
        DrawCircleV(particles[i].pos, 5, BLUE);
    }
}