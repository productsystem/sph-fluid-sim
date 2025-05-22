#include "sph.h"
#include "raylib.h"
#include "raymath.h"
#include "constants.h"
#include "particle.h"
#include <math.h>

Vector2 mouseLastPos = {0};
#define MAX_PICKED 32
int pickedCount = 0;
int pickedIndices[MAX_PICKED];
static Vector2 prevMousePos = {0};
static bool dragging = false;

void Integrate()
{
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].vel = Vector2Add(particles[i].vel, Vector2Scale((Vector2){particles[i].force.x/particles[i].rho, particles[i].force.y/particles[i].rho}, DT));
        particles[i].pos = Vector2Add(particles[i].pos, Vector2Scale(particles[i].vel, DT));

        if(particles[i].pos.x - EPS < 0)
        {
            particles[i].vel.x *= DAMPING;
            particles[i].pos.x = EPS;
        }
        if(particles[i].pos.x + EPS > GetScreenWidth())
        {
            particles[i].vel.x *= DAMPING;
            particles[i].pos.x = GetScreenWidth() - EPS;
        }
        if(particles[i].pos.y - EPS < 0)
        {
            particles[i].vel.y *= DAMPING;
            particles[i].pos.y = EPS;
        }
        if(particles[i].pos.y+ EPS > GetScreenHeight())
        {
            particles[i].vel.y *= DAMPING;
            particles[i].pos.y = GetScreenHeight() - EPS;
        }
    }
}

void ComputeDensityPressure()
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].rho = 0.f;
        int cx = (int)(particles[i].pos.x / CELL_SIZE);
        int cy = (int)(particles[i].pos.y / CELL_SIZE);

        for (int dx = -1; dx <= 1; dx++)
        {
            for(int dy = -1; dy <= 1; dy++)
            {
                int hash = SpatialHash((Vector2){(cx + dx) * CELL_SIZE, (cy + dy) * CELL_SIZE});
                Cell cell = hashTable[hash];

                for(int k = 0; k <cell.count; k++)
                {
                    Particle *p = cell.particles[k];
                    float r2 = Vector2DistanceSqr(particles[i].pos, p->pos);
                    if (r2 < H * H)
                    {
                        particles[i].rho += MASS * POLY6 * pow(H * H - r2, 3);
                    }
                    
                }
            }
        }
        particles[i].p = GAS_CONSTANT * (particles[i].rho - REST_DENSTIY);
    }
}

void ComputeForces()
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Vector2 fp = {0};
        Vector2 fv = {0};
        if(particles[i].rho == 0.f)
            continue;
        Vector2 fg = Vector2Scale(G, -MASS / particles[i].rho);

        int cx = (int)(particles[i].pos.x / CELL_SIZE);
        int cy = (int)(particles[i].pos.y / CELL_SIZE);

        for (int dx = -1; dx <= 1; dx++)
        {
            for(int dy = -1; dy <= 1; dy++)
            {
                int hash = SpatialHash((Vector2){(cx + dx) * CELL_SIZE, (cy + dy) * CELL_SIZE});
                Cell cell = hashTable[hash];

                for(int k = 0; k <cell.count; k++)
                {
                    Particle *p = cell.particles[k];
                    if(&particles[i] == p || p->rho == 0.f)
                    {
                        continue;
                    }
                    float r = Vector2Distance(particles[i].pos, p->pos);
                    if(r < H)
                    {
                        Vector2 rij = Vector2Subtract(p->pos, particles[i].pos);
                        Vector2 dir = Vector2Normalize(rij);

                        float pCalc = -MASS * (particles[i].p + p->p) / (2.0f * p->rho);
                        fp = Vector2Add(fp, Vector2Scale(dir, pCalc * SPIKY_GRAD * pow(H - r, 3.0f)));

                        Vector2 velDiff =  Vector2Subtract(p->vel, particles[i].vel);
                        float vCalc = VISCOSITY * MASS / p->rho;
                        fv = Vector2Add(fv, Vector2Scale(velDiff, vCalc * VISC_LAP * (H - r)));
                    }
                }
            }
        }

        particles[i].force = Vector2Add(fp, Vector2Add(fv, fg));
    }
}

void HandleMouseInteraction()
{
    Vector2 mousePos = GetMousePosition();
    Vector2 mouseDelta = Vector2Subtract(mousePos, prevMousePos);
    float radius = 30.0f;
    float springStrength = 200.0f;
    float dampingStrength = 0.8f;
    float throwImpulseScale = 100.0f;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        pickedCount = 0;
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (Vector2Distance(particles[i].pos, mousePos) < radius && pickedCount < MAX_PICKED)
            {
                pickedIndices[pickedCount++] = i;
            }
        }
        dragging = true;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && dragging)
    {
        for (int i = 0; i < pickedCount; i++)
        {
            int index = pickedIndices[i];
            Vector2 toMouse = Vector2Subtract(mousePos, particles[index].pos);
            Vector2 damping = Vector2Scale(particles[index].vel, -dampingStrength);
            Vector2 springForce = Vector2Add(Vector2Scale(toMouse, springStrength), damping);
            particles[index].force = Vector2Add(particles[index].force, springForce);
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && dragging)
    {
        for (int i = 0; i < pickedCount; i++)
        {
            int index = pickedIndices[i];
            Vector2 impulse = Vector2Scale(mouseDelta, throwImpulseScale);
            particles[index].vel = Vector2Add(particles[index].vel, impulse);
        }
        pickedCount = 0;
        dragging = false;
    }

    prevMousePos = mousePos;
}


void Update()
{
    BuildHashTable();
    ComputeDensityPressure();
    ComputeForces();
    HandleMouseInteraction();
    Integrate();
}