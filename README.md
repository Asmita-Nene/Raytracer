# Raytracer
 
A CPU-based ray tracer built in C++, following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series. This is a personal learning project focused on understanding rendering systems, C++ architecture, and building a codebase that can scale toward GPU-based and distributed rendering.

## Motivation
 
Started this to get hands-on with systems-level C++ and eventually CUDA. The goal wasn't just to follow the book — it was to build something structured well enough that adding GPU kernels, multithreading, or a distributed rendering layer later doesn't require rewriting everything.
 
 
## Current State
 
The renderer currently supports diffuse and metal materials with physically-based reflection. Metal supports both isotropic (uniform roughness) and anisotropic (directional roughness) modes.
 
Milestones planned:
- [x] Core ray-object intersection
- [x] Normal-based shading
- [x] Diffuse materials
- [x] Metal materials with isotropic roughness
- [x] Metal materials with anisotropic roughness (see known limitations)
- [x] Dielectric / glass materials
- [ ] Point lights and shadows
- [ ] BVH acceleration structure
- [ ] Multithreading
- [ ] CUDA / GPU rendering

## Known Limitations

Anisotropic metal on curved surfaces: The anisotropic roughness model stretches highlights into directional streaks (like brushed metal), but the effect is not visually obvious on spheres. This is because the tangent frame (T, B vectors) is recomputed from the surface normal at each hit point, and on a sphere the normal rotates continuously across the surface — so the streak direction rotates with it rather than remaining consistent. On flat geometry the effect is pronounced and correct. The fix for this is implementation UV-based tangents that derive T from the UV gradient direction on the surface, giving a consistent streak direction tied to the geometry, and will be implemented in further commits.

Glass(Dielectric) Material: The black rim is caused by Total Internal Reflection (TIR) at grazing angles causing the rays to reflect internally rather than exit. These rays bounce repeatedly inside the sphere via TIR until they reach the maximum recursion depth and return black. This is physically accurate behavior, it will be later optimised for proper visual representation in further commits.
 
## Architecture

The codebase is structured around clear separation of concerns — each class has one job.
A few intentional design decisions:

- **`Renderer`** owns the render loop but delegates everything — ray generation to `Camera`, intersection to `Scene`, output to `Image`. This makes it easy to swap in a CUDA kernel later without touching shading logic.
- **`Primitive`** is an abstract base class, so adding geometry (triangles, meshes) doesn't touch existing code.
- **`Material`** is an abstract base class — `Diffuse`, `Metal`, and `Dielectric` implement `getRay()` and `getAlbedo()` so the renderer never needs to know which material it's dealing with.
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
│       ├── Material.cpp / .hpp
│       ├── Primitive.cpp / .hpp
│       ├── Renderer.cpp / .hpp
│       ├── Scene.cpp / .hpp
│       ├── UtilityClasses.cpp / .hpp
│       └── RaytracerBasics.cpp
└── GPU-Based-Rendering/           # Planned
```
 
## Building
 
Open `RaytracerBasics.sln` in Visual Studio and build with `Ctrl+Shift+B`.
 
 
## Reference
 
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) — Peter Shirley
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
