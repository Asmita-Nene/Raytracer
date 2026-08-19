# Raytracer
 
A CPU-based ray tracer built in C++, following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series. This is a personal learning project focused on understanding rendering systems, C++ architecture, and building a codebase that can scale toward GPU-based and distributed rendering.

## Motivation
 
Started this to get hands-on with systems-level C++ and eventually CUDA. The goal wasn't just to follow the book — it was to build something structured well enough that adding GPU kernels, multithreading, or a distributed rendering layer later doesn't require rewriting everything.
 
 ## Current State

The renderer currently supports diffuse, metal, and dielectric materials with physically-based scattering. Metal supports both isotropic (uniform roughness) and anisotropic (directional roughness) modes; dielectric materials model refraction using Schlick's approximation, Beer's Law absorption, and total internal reflection.

Scene primitives include spheres and triangles (Möller–Trumbore ray-triangle intersection). Triangle meshes can be loaded directly from Wavefront `.obj` files (vertex positions and normals). Currently verified the working by exporting the Blender Suzanne model as obj and the obtained render is present in the Images folder.

Implemented Bounding Volume Hierarchy and Surface Area Heuristic Algorithm to optimize the BVH tree and speed up rendering.

Milestones planned:
- [x] Core ray-object intersection
- [x] Normal-based shading
- [x] Diffuse materials
- [x] Metal materials with isotropic roughness
- [x] Metal materials with anisotropic roughness (see known limitations)
- [x] Dielectric / glass materials
- [x] BVH acceleration structure(with SAH)
- [X] Triangle Primitive
- [X] Obj file loader system
- [ ] Point lights and shadows
- [ ] CUDA / GPU rendering



## Known Limitations

Anisotropic metal on curved surfaces: The anisotropic roughness model stretches highlights into directional streaks (like brushed metal), but the effect is not visually obvious on spheres. This is because the tangent frame (T, B vectors) is recomputed from the surface normal at each hit point, and on a sphere the normal rotates continuously across the surface — so the streak direction rotates with it rather than remaining consistent. On flat geometry the effect is pronounced and correct. The fix for this is implementation UV-based tangents that derive T from the UV gradient direction on the surface, giving a consistent streak direction tied to the geometry, and will be implemented in further commits.

Glass(Dielectric) Material: The black rim is caused by Total Internal Reflection (TIR) at grazing angles causing the rays to reflect internally rather than exit. These rays bounce repeatedly inside the sphere via TIR until they reach the maximum recursion depth and return black. This is physically accurate behavior, it will be later optimised for proper visual representation in further commits.
 
## Architecture
The codebase is structured around clear separation of concerns — each class has one job.
A few intentional design decisions:
- **`Renderer`** owns the render loop but delegates everything — ray generation to `Camera`, intersection to `Scene`, output to `Image`. This makes it easy to swap in a CUDA kernel later without touching shading logic.
- **`Primitive`** is an abstract base class, so adding geometry doesn't touch existing code — `Sphere` and `Triangle` (Möller–Trumbore intersection) both implement it without either knowing the other exists.
- **`Material`** is an abstract base class — `Diffuse`, `Metal`, and `Dielectric` implement `getRay()` and `getAlbedo()` so the renderer never needs to know which material it's dealing with.
- **`HitRecord`** is a plain data struct, not owned by any object — keeps intersection and shading decoupled.
- **`Scene`** owns acceleration in addition to primitive storage: it builds a surface-area-heuristic BVH over its `Primitive` list and handles traversal internally, so `Renderer` just calls `Scene::intersect()` without any awareness that a BVH exists underneath.
- **`AABB`** is a small, self-contained bounding-box type (slab-test intersection, box union) shared by both BVH construction and traversal — keeping this math isolated from `Primitive` and `Scene` logic makes it easy to unit-test in isolation.
- **`ObjFileHandle`** is a  parsing utility, it only reads a `.obj` file and constructs `Triangle` objects through `Scene`'s existing `addPrimitiveObject()` interface, so mesh loading required zero changes to `Primitive`, `Material`, or `Scene`'s own API.
- `Point3` and `Color` are type aliases for `Vector3`, which keeps the math layer clean without unnecessary wrapper classes.

## Project Structure
```
Raytracer/
├── CPU-Based-Rendering/
│   └── RaytracerBasics/
│       ├── AABB.cpp / .hpp
│       ├── BVHNode.hpp
│       ├── Camera.cpp / .hpp
│       ├── HitRecord.hpp
│       ├── ImageClasses.cpp / .hpp
│       ├── Material.cpp / .hpp
│       ├── ObjFileHandle.cpp / .hpp
│       ├── Primitive.cpp / .hpp
│       ├── Renderer.cpp / .hpp
│       ├── Scene.cpp / .hpp
│       ├── UtilityClasses.cpp / .hpp
│       └── RaytracerBasics.cpp
└── GPU-Based-Rendering/           # Planned
```
 
 
## Reference
 
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) — Peter Shirley
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
