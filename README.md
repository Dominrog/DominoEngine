# **Domino Engine**

The engine is based on an **Entity–Component–System (ECS)** architecture.

Entities are represented by 32-bit unsigned integers (`uint32_t`) and serve solely as unique identifiers.  
They do not store data themselves, but are associated with one or more components.

Components are plain data containers and are managed by the **registry**, which is also responsible for creating and assigning entity IDs.  
All component logic is implemented in the corresponding **systems**.

Systems operate on specific component types and are updated once per frame.

---

## Scripting

Lua scripts are implemented as components and therefore follow the same lifecycle rules as other components.  
Each script component is processed by a dedicated scripting system.

Scripts provide the following callback methods:

- `onStart()` - called once when the entity is initialized
- `onUpdate(dt)` - called every frame  
  (`dt` is the delta time, i.e. the time elapsed since the previous frame)

Through scripts, component data of the owning entity can be read and modified.
Additionally, by using **EntityLink** components, you can access the components of other entities.  
For example, you can get the transform of a linked entity using:

```lua
local targetTransform = getTransform(link.target)
```

Scripts have access to the InputSystem via lua bindings. The mouse position is available through 
`mouseDX()` and `mouseDY()`. Key bindings are available with the `isDown(...)` method.
For example:

```lua
local dx = input.mouseDX()

if input.isDown("W") then
  ...
end
```

---

## Systems

The engine currently provides **seven** core systems:

- **CameraSystem**  
  Updates all `Camera` components and computes the corresponding view and projection matrices.

- **InputSystem**  
  Captures all user input (keyboard, mouse, scroll) and updates the `InputState` accordingly. Ensures that current states—such as pressed keys, mouse movement (`dx`, `dy`), and scroll values—are available for scripts or other systems.

- **RenderSystem**  
  Responsible for uploading, updating, and rendering GPU data associated with `Mesh` components.

- **ScriptSystem**  
  Manages Lua bindings and executes all `Script` components during their lifecycle callbacks.

- **TransformSystem**  
  Updates all `Transform` components and propagates spatial changes to dependent systems.

- **PhysicsSystem**
  Adds calculated forces to an entities force accumulator. Requires the `Physics` component.

- **CollisionSystem**  
  Processes all box colliders in the scene and detects collisions between them.

---

## Components

The engine currently provides **nine** core components:

- **Info**  
  Stores metadata associated with an entity.  
  Currently limited to a name.

- **Transform**  
  Contains the spatial data of an entity, including position, rotation, and scale.  
  Available in the script as `transform`.

- **Physics**
  Stores the entities physical data such as mass and velocity.

- **Mesh**  
  Stores all mesh-related data, including VAO, VBO, and EBO handles, as well as the associated vertex and index data.

- **BoxCollider**   
  Stores data for a box-shaped collider, including its full size, local offset, collision layer and mask, and whether the collider acts as a trigger.   
  Available in the script as `box`.

- **Script**  
  Stores the file path to the associated Lua script.

- **EntityLink**  
  Stores a reference to another entity using its entity ID.  
  This allows one entity to safely access and interact with the components of another entity.   
  Available in the script as `link`.

- **Camera**  
  Contains all data required to define the camera’s position and orientation in world space.    
  Available in the script as `camera`.

- **Shader**  
  Holds a reference to the shader object used for rendering the entity.

---

## Importing mesh data

Meshes can be loaded from glTF 2.0 binary files (`.glb`).  
To import a mesh, create a `MeshLoader` instance (usually already provided by the engine itself) and call:

```cpp
mesh_loader.importMesh("path/to/model.glb")
```

---

## Work in progress

### high priority

- Add angular transforms after collisions

- replace `keyFromString()` in `LuaBindings.h` with an unordered KeyMap

### low priority

- allow the `EntityLink` to link more than one entity

- add material component allowing to add a color and a texture to the mesh

- a savefile to save scenes

- gui implementation to create entities and components during runtime in the engine

- improve the `CollisionSystem` by implementing a spatial hash grid

## Issues

- The `CollisionSystem` sometimes registers multiple collisions, can be fixed with logic by checking previous state

## Third-Party

- GLFW, GLAD, GLM, Dear ImGui, Lua, tinygltf, stb_image, FastNoiseLite, KHR