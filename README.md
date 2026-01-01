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

---

## Systems

The engine currently provides **five** core systems:

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

---

## Components

The engine currently provides **seven** core components:

- **Info**  
  Stores metadata associated with an entity.  
  Currently limited to a name.

- **Transform**  
  Contains the spatial data of an entity, including position, rotation, and scale.

- **Mesh**  
  Stores all mesh-related data, including VAO, VBO, and EBO handles, as well as the associated vertex and index data.

  ### Importing mesh data

  Meshes can be loaded from glTF 2.0 binary files (`.glb`).  
  To import a mesh, create a `MeshLoader` instance and call:

  ```cpp
  mesh_loader.importMesh("path/to/model.glb")
  ```

- **Script**  
  Stores the file path to the associated Lua script.

- **EntityLink**  
  Stores a reference to another entity using its entity ID.  
  This allows one entity to safely access and interact with the components of another entity.

- **Camera**  
  Contains all data required to define the camera’s position and orientation in world space.

- **Shader**  
  Holds a reference to the shader object used for rendering the entity.

---

## Work in progress

### high priority

- replace `keyFromString()` in `LuaBindings.h` with an unordered KeyMap

### low priority

- implementation of scenes in safefiles

- a savefile to save scenes

- gui implementation to create entities and components during runtime in the engine