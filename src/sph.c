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

        for (int j = 0; j < MAX_PARTICLES; j++)
        {
            float r2 = Vector2DistanceSqr(particles[i].pos, particles[j].pos);
            if (r2 < H * H)
            {
                particles[i].rho += MASS * POLY6 * powf(H * H - r2, 3);
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

        for (int j = 0; j < MAX_PARTICLES; j++)
        {
            if (i == j || particles[i].rho == 0.f || particles[j].rho == 0.f)
                continue;

            float r = Vector2Distance(particles[i].pos, particles[j].pos);
            if (r >= H)
                continue;

            Vector2 rij = Vector2Subtract(particles[j].pos, particles[i].pos);
            Vector2 dir = Vector2Normalize(rij);
            float pCalc = -MASS * (particles[i].p + particles[j].p) / (2.0f * particles[j].rho);
            fp = Vector2Add(fp, Vector2Scale(dir, pCalc * SPIKY_GRAD * powf(H - r, 3.f)));
            Vector2 velDiff = Vector2Subtract(particles[j].vel, particles[i].vel);
            float vCalc = VISCOSITY * MASS / particles[j].rho;
            fv = Vector2Add(fv, Vector2Scale(velDiff, vCalc * (H - r) * VISC_LAP));
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
    ComputeDensityPressure();
    ComputeForces();
    HandleMouseInteraction();
    Integrate();
}