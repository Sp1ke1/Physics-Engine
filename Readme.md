Physics-Engine — Report for the "Physics Programmer" task
=========================================================

Objective
---------
Develop a 3D simulation of bouncing balls inside a confined playfield (mantinels). The physics must be implemented from scratch without using external physics engines.

Summary of the implementation
-----------------------------
This repository implements a simple 3D physics system for spheres and axis-aligned box boundaries. Main features:

- Full 3D scene rendered with raylib
- Confined playfield represented by six thin static boxes (mantinels)
- Gravity along the negative Y axis
- Multiple balls with properties: mass, linear velocity, angular velocity, friction
- Collision detection: sphere–sphere and sphere–box (axis-aligned box)
- Collision response: normal impulse, approximate tangential impulse (friction) and tangential impulse influence on angular velocity
- Simple friction model (linear/angular damping and Coulomb clamp for tangential impulse)

Where to look in the code
-------------------------
- Scene main logic: `PhysicsEngine/Source/Scene.cpp`
- Collision detection: `PhysicsEngine/Source/Collision.cpp`
- Shape definitions: `PhysicsEngine/Include/Shape.hpp`
- Physics body definition: `PhysicsEngine/Include/PhysicsBody.hpp`
- Collision tests: `Test_Main.cpp`
- Build configuration: `CMakeLists.txt`

Physics principles applied
--------------------------
1) Integration
- Semi-implicit integration: velocities are updated with gravity and damping, then positions are integrated from velocities.

2) Gravity
- A constant acceleration is applied along the negative Y axis (configurable in `SSceneParameters`).

3) Collisions
- Two detection methods:
	- Sphere–sphere: using the sum of radii; normal is the vector between centers; contact point is computed on the surfaces.
	- Sphere–box: closest point on the box + distance test; when the sphere center lies inside the box, the nearest face is selected.

4) Collision response
- Normal impulse: computed using coefficient of restitution and summed inverse masses; applied as an instantaneous change to linear velocities.
- Tangential (friction) impulse: approximated using inverse masses and rotational contributions in the form r^2 / I (scalar approximation). jt is clamped using Coulomb friction: |jt| <= mu * j_normal.
- Angular velocity: modified by the tangential impulse (torque = r x Jt), where r is the contact offset and I is the scalar moment of inertia (see `SShape::GetMomentOfInertia`).

5) Friction
- Two mechanisms apply friction:
	- Explicit damping: linear and angular damping (exponential decay) applied each step to gradually reduce motion.
	- Tangential impulse at contact: reduces relative tangential sliding and generates spin.

Architectural and numeric simplifications
----------------------------------------
- A scalar moment-of-inertia approximation is used (no full 3×3 inertia tensor or world-space inertia transforms). This reduces accuracy for oriented boxes but greatly simplifies computations.
- Discrete collision handling (no CCD). At high speeds tunneling may occur. Mitigations: smaller time steps, multiple solver iterations, thicker mantinels, or swept/backtracking tests.

Build and run instructions
--------------------------
Requirements: CMake (>=3.14), a C++20-capable compiler, network access for FetchContent (raylib, googletest).

Example using PowerShell on Windows:

    mkdir build; cd build; cmake ..
    cmake --build . --config Debug

Running tests (from the build directory):

    ctest -C Debug --output-on-failure

Running the example (after build):

    <build-folder>\Debug\PhysicsEngineExample.exe

Note: If you encounter CRT/runtime mismatches when linking tests on MSVC, check the `gtest_force_shared_crt` setting in `CMakeLists.txt` and align runtime flags across targets.

Testing
-------
`Test_Main.cpp` contains unit tests for collision detection that verify absence/presence of collisions, penetration depth, normals and contact points for both sphere–sphere and sphere–box cases.

Performance and optimizations for large-scale simulations
--------------------------------------------------------
If the goal is thousands of balls in a large field, consider the following optimizations:

1) Broad-phase spatial partitioning
   - Uniform grid / spatial hash — cheap and effective for uniformly distributed objects.
   - Sweep-and-prune or BVH — fewer candidate pairs for dense or clustered scenes.

2) Batching and SIMD
   - Batch independent computations (vectorize collision batches) to exploit SIMD.

3) Multithreading
   - Parallelize integration and broad-phase.
   - For narrow-phase and impulse application use accumulation techniques to avoid data races (e.g., per-body impulse buffers and a merge step).

4) Reduce allocations
   - Use object pools and preallocated buffers for bodies and temporary arrays.

5) Simplified LOD physics
   - Use cheaper approximations for distant or low-importance objects.

Correctness assessment
----------------------
- Gravity and core kinematics are implemented correctly using semi-implicit integration.
- Sphere–sphere and sphere–box collision detection return correct normals, penetration amounts and contact points in covered test cases.
- Collision response includes normal impulse and an approximate friction model. Angular dynamics are modelled with scalar moments of inertia — visible spin behaviour is produced, but full physical accuracy for arbitrarily oriented boxes requires a full inertia tensor.

Further improvements
--------------------
- Implement full 3×3 inertia tensor and world-space transforms for accurate box rotation.
- Add Continuous Collision Detection (CCD) to prevent tunneling at high velocities.
- Implement a more accurate friction model (including normal-force-dependent friction and velocity-dependent friction coefficients).
- Parallelize broad-phase and solver for large-scale real-time simulations.