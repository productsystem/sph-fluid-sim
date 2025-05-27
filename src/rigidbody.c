#include "rigidbody.h"
#include "constants.h"
#include "particle.h"
#include "raymath.h"
#include <math.h>

void InitRigidBody(RigidBody *rb, Vector2 pos, float width, float height, float mass)
{
    rb -> pos = pos;
    rb -> vel = (Vector2){0};
    rb -> width = width;
    rb -> height = height;
    rb -> mass = mass;
}

void UpdateRigidBody(RigidBody *rb, float dt)
{
    rb -> vel = Vector2Add(rb->vel, Vector2Scale(G,-3000 * dt));
    rb -> pos = Vector2Add(rb ->pos , Vector2Scale(rb->vel,dt));
}

void ApplyForce(RigidBody *rb, Vector2 force)
{
    float dampingCoef = 0.5f;
    Vector2 dampingForce = Vector2Scale(rb->vel, -dampingCoef);
    force = Vector2Add(force, dampingForce);
    Vector2 acc = Vector2Scale(force, 1.0f/(rb -> mass));
    rb->vel = Vector2Add(rb->vel, acc);
}

bool IsPointInBox(Vector2 point, RigidBody *rb)
{
    return (point.x > rb->pos.x - rb->width/2 && point.x < rb->pos.x + rb->width/2 && point.y > rb->pos.y - rb->height/2 &&point.y < rb->pos.y + rb->height/2);
}

void ApplyBuoyancy(RigidBody *rb)
{
    int sub = 0;
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if(IsPointInBox(particles[i].pos, rb))
        {
            sub++;
        }
    }
    if(!sub) return;

    float displaced = sub * MASS/REST_DENSTIY;
    Vector2 buoyancy = Vector2Scale(G, 10000 * displaced);

    ApplyForce(rb,buoyancy);
}

void DrawRigidBody(RigidBody *rb)
{
    DrawRectangleV(rb->pos,(Vector2){rb->width,rb->height},GREEN);
}