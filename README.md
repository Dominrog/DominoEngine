# **Domino Engine**

The engine architecture is based on an **Entity–Component–System**.

Entities are represented by 32-bit unsigned integers and serve solely as unique identifiers.  
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

Through scripts, component data can be read and modified.
You can either modify component data of the script owner or reference a different entity.
For example, using:

```lua
local targetTransform = getTransform(target)
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

The engines core systems:

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
  Uses Jolt for physics.

---

## Components

The engines core components:

- **Info**  
  Stores metadata associated with an entity.  
  Currently limited to a name.

- **Transform**  
  Contains the spatial data of an entity, including position, rotation, and scale.  
  Available in the script as `transform`.

- **Mesh**  
  Stores all mesh-related data, including VAO, VBO, and EBO handles, as well as the associated vertex and index data.

- **Collider**   
  Stores Jolt collider data.

- **Script**  
  Stores the file path to the associated Lua script.

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


### low priority

- add material component allowing to add a color and a texture to the mesh

- a savefile to save scenes

- gui implementation to create entities and components during runtime in the engine

## Issues


## Third-Party

- GLFW, GLAD, GLM, Dear ImGui, Lua, tinygltf, stb_image, FastNoiseLite, KHR, JoltPhysics