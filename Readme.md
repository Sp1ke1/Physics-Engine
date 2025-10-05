Physics-Engine
=========================================================

Summary of the implementation
-----------------------------
This repository implements a simple 3D physics system for spheres and axis-aligned box boundaries. Main features:

- Full 3D scene rendered with raylib
- Confined playfield represented by six thin static boxes (mantinels)
- Gravity along the negative Y axis
- Multiple balls with properties: mass, linear velocity, angular velocity, friction
- Configurable simulation with substepping and fixed tickrate. 
- Collision detection: sphere–sphere and sphere–box (axis-aligned box)
- Collision response: normal impulse, tangential impulse (friction) and tangential impulse influence on angular velocity
- Simple friction model (linear/angular damping and Coulomb clamp for tangential impulse)

Controls 
-------------------------

- WASD - Movement
- Mouse | EQ | Arrows | - Look around
- Enter - Pause
- R - Restart


Configration
-------------------------
- To configure the simulation you can change values inside of the PhysicsEngine\Include\Parameters.hpp. There are number of exposed parameters such as number of balls, number of solver steps, damping\friction\restitution coefficients etc. 

Where to look in the code
-------------------------
- Scene main logic: `PhysicsEngine/Source/Scene.cpp`
- Collision detection: `PhysicsEngine/Source/Collision.cpp`
- Tests: `Test_Main.cpp`
- Build configuration: `CMakeLists.txt`

Physics principles applied
--------------------------
1) Integration
- Semi-implicit integration: velocities are updated with gravity and damping, then positions are integrated from velocities.

2) Gravity
- A constant acceleration is applied along the negative Y axis. 

3) Collisions
- Two detection methods:
	- Sphere–sphere.
	- Sphere–box.

4) Collision response
- Positional correction: object positions are instantly corrected and moved from each other. 
- Normal impulse: computed using coefficient of restitution and summed inverse masses;
- Tangential (friction) impulse: computed using coefficient of friction and summed inverse masses; Clamped using Coulomb friction: |jt| <= mu * j_normal.
- Linear velocity: modified by the impulse (dV = ( J * InvMass ) ), where J is the total impulse and InvMass is the 
- Angular velocity: modified by the impulse (dV = ( r x J ) / I ), where r is the contact offset, J is the total impulse and I is the scalar moment of inertia.

5) Friction
- Two mechanisms apply friction:
	- Explicit damping: linear and angular damping (exponential decay) applied each step to gradually reduce motion.
	- Tangential impulse (friction) at contact: generates spin.

Testing
-------
`Test_Main.cpp` contains unit tests for collision detection that verify absence/presence of collisions, penetration depth, normals and contact points for both sphere–sphere and sphere–box cases.


Future improvements 
-----------------------------

**Physics**
- Currently, the simulation does not account for uneven mass distribution among objects (inertia tensor). In the future, for more accurate simulation, it would be advisable to introduce these values, as well as, possibly, the center of mass offset. This would yield more realistic results for angular velocity updates. 

**Architectural changes**

To simplify things, the logic of the “physical engine” and the “user application” have essentially been combined in the PE::CScene class. Ideally, in a larger project, I would divide the entire application into three components:

- A static library of the physical engine with the PE::CPhysicsScene class. This library would include all physics-related code, collision processing, scene updates, and physics settings. 

- A static application library that would compose the physical scene, as well as a separate scene for rendering, windows, event handlers, etc. 

- Executable user code that would create its own application instance (or, for example, an abstract Layer that could be pushed into the application). And the user would generate a user-specific simulation themselves through methods of adding objects to the scene. 

This way, the physics engine code would be much more flexible and reusable. 

**Optimizations for large-scale simulation**

*Global optimizations*

- Collision processing is implemented through a simple O(n^2) iteration through all objects in the scene and resolution of collisions between them. To start with, a simple BroadPhase preprocessor could be implemented, which would collect all pairs of objects between which there is a collision and send them to NarrowPhase, where the collision responses would be calculated. Asymptotically, the algorithm would work the same, but in practice, I think this would speed up the simulation.

- As a next step, I would suggest creating an acceleration structure for finding collision pairs. Octree, BVH, SAP, or similar structures may be suitable here. This will significantly complicate the code, but it will greatly speed up the process of finding collision pairs in BroadPhase. Checks will not be performed between all objects, but only between objects that are approximately close to each other and are moving. We could also apply the simulation islands method by adding sleep and wake modes to objects and dividing the space into sleeping and woke, adding the ability to exclude regions where objects are not moving from processing and wake them up when a non-sleeping dynamic object approaches them.

*Code optimizations* 

- To begin with, I would suggest converting all mathematics to SIMD calculations using a more modern mathematical library (e.g., glm instead of raylibmath). All vector operations on modern processors will be accelerated up to 4x. 

- I would also suggest using multithreading. To start with, we can offload all physics calculations to a separate thread and synchronize them with the game/main thread via mutex. Later on, we can enable multithreading within the physics engine itself. For example, we can try to parallelize BroadPhase by issuing different non-intersecting regions from the physical thread pool to threads for processing. I would be careful here, as the cost of waiting for thread synchronization may exceed the benefits of parallelizing calculations. 


