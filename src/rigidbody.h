#pragma once

#include "raylib.h"

typedef struct 
{
    Vector2 pos;
    Vector2 vel;
    float width, height;
    float mass;
} RigidBody;

void InitRigidBody(RigidBody *rb, Vector2 pos, float width, float height, float mass);
void UpdateRigidBody(RigidBody *rb, float dt);
void ApplyForce(RigidBody *rb, Vector2 force);
bool IsPointInBox(Vector2 point, RigidBody *rb);
void ApplyBuoyancy(RigidBody *rb);
void DrawRigidBody(RigidBody *rb);
