#pragma once
#include "raylib.h"
#include <math.h>

#define G (Vector2){0.0f, -10.0f}
#define REST_DENSTIY 300.0f
#define GAS_CONSTANT 2000.0f
#define H 16.0f 
#define MASS 2.5f
#define VISCOSITY 200.0f 
#define DT 0.0007f 

#define POLY6 4.f / (PI * pow(H, 8.f))
#define VISC_LAP 40.f / (PI * pow(H, 5.f))
#define SPIKY_GRAD -10.f / (PI * pow(H, 5.f))
#define EPS H
#define DAMPING -0.5f 
