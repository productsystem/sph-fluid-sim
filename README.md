# Smoothed Particle Hydrodynamics
The project implements Smoothed Particle Hydrodynamics (SPH) techniques to simulates fluid behaviour. It uses a Lagrangian approach, so the fluid is modelled with particles.
The implementation is done in C using Raylib for visualization.

## Optimization
Since the SPH algorithm is expensive, spatial hashing is implemented to decrease the time complexity from O(n^2) to O(n).

## Running
The repository only has the src files, to run it under raylib, building from source is possible, or more effectively, using the raylib quickstart https://github.com/raylib-extras/raylib-quickstart template
is a very simple way to run it.
