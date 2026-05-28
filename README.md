# Raytracer
 
A CPU-based ray tracer built in C++, following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series. This is a personal learning project focused on understanding rendering systems, C++ architecture, and building a codebase that can scale toward GPU-based and distributed rendering.

## Motivation
 
Started this to get hands-on with systems-level C++ and eventually CUDA. The goal wasn't just to follow the book — it was to build something structured well enough that adding GPU kernels, multithreading, or a distributed rendering layer later doesn't require rewriting everything.
 
 
## Current State
 
The renderer currently produces a shaded sphere where pixel color is derived from surface normals. No materials or lighting yet — this is the first working pass before those systems get layered in.
 
Milestones planned:
- [x] Core ray-object intersection
- [x] Normal-based shading
- [ ] Diffuse materials
- [ ] Point lights and shadows
- [ ] Reflections and refractions
- [ ] BVH acceleration structure
- [ ] Multithreading
- [ ] CUDA / GPU rendering
 
## Architecture
 
The codebase is structured around clear separation of concerns — each class has one job.
A few intentional design decisions:
 
- **`Renderer`** owns the render loop but delegates everything — ray generation to `Camera`, intersection to `Scene`, output to `Image`. This makes it easy to swap in a CUDA kernel later without touching shading logic.
- **`Primitive`** is an abstract base class, so adding geometry (triangles, meshes) doesn't touch existing code.
- **`HitRecord`** is a plain data struct, not owned by any object — keeps intersection and shading decoupled.
- `Point3` and `Color` are type aliases for `Vector3`, which keeps the math layer clean without unnecessary wrapper classes.

 
## Project Structure
 
```
Raytracer/
├── CPU-Based-Rendering/
│   └── RaytracerBasics/
│       ├── Camera.cpp / .hpp
│       ├── HitRecord.hpp
│       ├── ImageClasses.cpp / .hpp
│       ├── Primitive.cpp / .hpp
│       ├── Renderer.cpp / .hpp
│       ├── Scene.cpp / .hpp
│       └── UtilityClasses.cpp / .hpp
└── GPU-Based-Rendering/           # Planned
```
 
 
## Building
 
Open `RaytracerBasics.sln` in Visual Studio and build with `Ctrl+Shift+B`.
 
 
## Reference
 
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) — Peter Shirley
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
