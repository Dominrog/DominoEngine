#include "LuaBindings.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <glm/gtc/quaternion.hpp>

#include <cstring>

#include "rigidbody.h"

static Registry* gRegistry = nullptr;
static InputState* gInput = nullptr;
static JPH::BodyInterface* gBodies = nullptr;


std::unordered_map<std::string, int> key_map = {
  {"W", GLFW_KEY_W},
  {"A", GLFW_KEY_A},
  {"S", GLFW_KEY_S},
  {"D", GLFW_KEY_D},
  {"Space", GLFW_KEY_SPACE}
};


void lua_setRegistry(Registry* registry)
{
  gRegistry = registry;
}

void lua_setInput(InputState* input)
{
  gInput = input;
}

void lua_setPhysics(JPH::BodyInterface* bodies)
{
  gBodies = bodies;
}


static int keyFromString(const char* key)
{
  auto it = key_map.find(key);
  if (it == key_map.end()) return -1;
  else return it->second;
}


static Transform* getTransformFromLua(lua_State* L)
{
  if (!gRegistry) {
    luaL_error(L, "Registry not set");
    return nullptr;
  }

  Entity e = (Entity)lua_tointeger(L, lua_upvalueindex(1));

  if (!gRegistry->has<Transform>(e)) {
    luaL_error(L, "Entity has no Transform");
    return nullptr;
  }

  return &gRegistry->get<Transform>(e);
}

static Camera* getCameraFromLua(lua_State* L)
{
  if (!gRegistry) {
    luaL_error(L, "Registry not set");
    return nullptr;
  }

  Entity e = (Entity)lua_tointeger(L, lua_upvalueindex(1));

  if (!gRegistry->has<Camera>(e)) {
    luaL_error(L, "Entity has no Camera");
    return nullptr;
  }

  return &gRegistry->get<Camera>(e);
}

static RigidBody* getRigidBody(Entity e)
{
  if (!gRegistry->has<RigidBody>(e))
    return nullptr;

  auto& body = gRegistry->get<RigidBody>(e);

  if (body.bodyID.IsInvalid())
    return nullptr;

  return &body;
}

static void syncTransformFromPhysics(Transform* tr, RigidBody* body)
{
  JPH::RVec3 position;
  JPH::Quat rotation;

  gBodies->GetPositionAndRotation(body->bodyID, position, rotation);

  tr->position = glm::vec3(
    (float)position.GetX(),
    (float)position.GetY(),
    (float)position.GetZ()
  );

  tr->rotationQ = glm::quat(
    rotation.GetW(),
    rotation.GetX(),
    rotation.GetY(),
    rotation.GetZ()
  );
}



extern "C" int l_input_isDown(lua_State* L)
{
  if (!gInput)
    return luaL_error(L, "InputState not set");

  const char* keyName = luaL_checkstring(L, 1);
  int key = keyFromString(keyName);

  if (key < 0) {
    lua_pushboolean(L, false);
    return 1;
  }

  lua_pushboolean(L, gInput->keys[key]);
  return 1;
}

extern "C" int l_input_mouseDX(lua_State* L)
{
  lua_pushnumber(L, gInput ? gInput->mouse_dx : 0.0f);
  return 1;
}

extern "C" int l_input_mouseDY(lua_State* L)
{
  lua_pushnumber(L, gInput ? gInput->mouse_dy : 0.0f);
  return 1;
}



extern "C" int transform_index(lua_State* L) 
{
  Transform* tr = getTransformFromLua(L);
  Entity e = (Entity)lua_tointeger(L, lua_upvalueindex(1));
  const char* key = luaL_checkstring(L, 2);

  RigidBody* body = getRigidBody(e);

  if (body) {
    if (!gBodies)
      return luaL_error(L, "Physics not set");

    syncTransformFromPhysics(tr, body);
  }

  glm::vec3 rotation = glm::degrees(
    glm::eulerAngles(glm::normalize(tr->rotationQ))
  );

  if (strcmp(key, "x") == 0) lua_pushnumber(L, tr->position.x);
  else if (strcmp(key, "y") == 0) lua_pushnumber(L, tr->position.y);
  else if (strcmp(key, "z") == 0) lua_pushnumber(L, tr->position.z);
  else if (strcmp(key, "rx") == 0) lua_pushnumber(L, rotation.x);
  else if (strcmp(key, "ry") == 0) lua_pushnumber(L, rotation.y);
  else if (strcmp(key, "rz") == 0) lua_pushnumber(L, rotation.z);
  else if (strcmp(key, "sx") == 0) lua_pushnumber(L, tr->scale.x);
  else if (strcmp(key, "sy") == 0) lua_pushnumber(L, tr->scale.y);
  else if (strcmp(key, "sz") == 0) lua_pushnumber(L, tr->scale.z);
  else lua_pushnil(L);

  return 1;
}

extern "C" int transform_newindex(lua_State* L) 
{
  Transform* tr = getTransformFromLua(L);
  Entity e = (Entity)lua_tointeger(L, lua_upvalueindex(1));
  const char* key = luaL_checkstring(L, 2);
  float value = (float)luaL_checknumber(L, 3);

  bool positionChanged =
    strcmp(key, "x") == 0 ||
    strcmp(key, "y") == 0 ||
    strcmp(key, "z") == 0;

  bool rotationChanged =
    strcmp(key, "rx") == 0 ||
    strcmp(key, "ry") == 0 ||
    strcmp(key, "rz") == 0;

  bool scaleChanged =
    strcmp(key, "sx") == 0 ||
    strcmp(key, "sy") == 0 ||
    strcmp(key, "sz") == 0;

  if (!positionChanged && !rotationChanged && !scaleChanged)
    return luaL_error(L, "Unknown Transform property: %s", key);

  RigidBody* body = getRigidBody(e);

  if (body) {
    if (!gBodies)
      return luaL_error(L, "Physics not set");

    if (scaleChanged)
      return luaL_error(L, "Scaling an existing physics body is not supported");

    syncTransformFromPhysics(tr, body);
  }

  if (strcmp(key, "x") == 0) tr->position.x = value;
  else if (strcmp(key, "y") == 0) tr->position.y = value;
  else if (strcmp(key, "z") == 0) tr->position.z = value;
  else if (strcmp(key, "sx") == 0) tr->scale.x = value;
  else if (strcmp(key, "sy") == 0) tr->scale.y = value;
  else if (strcmp(key, "sz") == 0) tr->scale.z = value;

  if (rotationChanged) {
    glm::vec3 rotation = glm::degrees(
      glm::eulerAngles(glm::normalize(tr->rotationQ))
    );

    if (strcmp(key, "rx") == 0) rotation.x = value;
    else if (strcmp(key, "ry") == 0) rotation.y = value;
    else if (strcmp(key, "rz") == 0) rotation.z = value;

    glm::quat qx = glm::angleAxis(
      glm::radians(rotation.x), glm::vec3(1, 0, 0)
    );
    glm::quat qy = glm::angleAxis(
      glm::radians(rotation.y), glm::vec3(0, 1, 0)
    );
    glm::quat qz = glm::angleAxis(
      glm::radians(rotation.z), glm::vec3(0, 0, 1)
    );

    tr->rotation = rotation;
    tr->rotationQ = glm::normalize(qz * qy * qx);
  }

  if (body && (positionChanged || rotationChanged)) {
    glm::quat rotation = glm::normalize(tr->rotationQ);

    JPH::EActivation activation =
      gBodies->GetMotionType(body->bodyID) == JPH::EMotionType::Static
      ? JPH::EActivation::DontActivate
      : JPH::EActivation::Activate;

    gBodies->SetPositionAndRotation(
      body->bodyID,
      JPH::RVec3(tr->position.x, tr->position.y, tr->position.z),
      JPH::Quat(rotation.x, rotation.y, rotation.z, rotation.w),
      activation
    );
  }

  return 0;
}

extern "C" int l_getTransform(lua_State* L) 
{
  Entity e = (Entity)luaL_checkinteger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");
  
  if (!gRegistry->has<Transform>(e)) {
    lua_pushnil(L);
    return 1;
  }
  
  lua_newtable(L);

  // Metatable 
  lua_newtable(L);

  // __index
  lua_pushinteger(L, (lua_Integer)e);
  lua_pushcclosure(L, transform_index, 1);
  lua_setfield(L, -2, "__index");

  // __newindex
  lua_pushinteger(L, (lua_Integer)e);
  lua_pushcclosure(L, transform_newindex, 1);
  lua_setfield(L, -2, "__newindex");

  lua_setmetatable(L, -2);

  return 1;
}



extern "C" int camera_index(lua_State* L)
{
  Camera* cam = getCameraFromLua(L);
  const char* key = luaL_checkstring(L, 2);

  if (strcmp(key, "yaw") == 0) lua_pushnumber(L, cam->yaw);
  else if (strcmp(key, "pitch") == 0) lua_pushnumber(L, cam->pitch);
  else if (strcmp(key, "movement_speed") == 0 || strcmp(key, "speed") == 0) lua_pushnumber(L, cam->movement_speed);
  else if (strcmp(key, "mouse_sensitivity") == 0 || strcmp(key, "sensitivity") == 0) lua_pushnumber(L, cam->mouse_sensitivity);
  else if (strcmp(key, "zoom") == 0) lua_pushnumber(L, cam->zoom);

  else if (strcmp(key, "front_x") == 0) lua_pushnumber(L, cam->front.x);
  else if (strcmp(key, "front_y") == 0) lua_pushnumber(L, cam->front.y);
  else if (strcmp(key, "front_z") == 0) lua_pushnumber(L, cam->front.z);

  else if (strcmp(key, "up_x") == 0) lua_pushnumber(L, cam->up.x);
  else if (strcmp(key, "up_y") == 0) lua_pushnumber(L, cam->up.y);
  else if (strcmp(key, "up_z") == 0) lua_pushnumber(L, cam->up.z);

  else if (strcmp(key, "right_x") == 0) lua_pushnumber(L, cam->right.x);
  else if (strcmp(key, "right_y") == 0) lua_pushnumber(L, cam->right.y);
  else if (strcmp(key, "right_z") == 0) lua_pushnumber(L, cam->right.z);
  else lua_pushnil(L);

  return 1;
}

extern "C" int camera_newindex(lua_State* L)
{
  Camera* cam = getCameraFromLua(L);
  const char* key = luaL_checkstring(L, 2);
  float value = (float)luaL_checknumber(L, 3);

  if (strcmp(key, "yaw") == 0) cam->yaw = value;
  else if (strcmp(key, "pitch") == 0) cam->pitch = glm::clamp(value, -89.0f, 89.0f);
  else if (strcmp(key, "zoom") == 0) cam->zoom = glm::clamp(value, 1.0f, 90.0f);
  else if (strcmp(key, "movement_speed") == 0 || strcmp(key, "speed") == 0) cam->movement_speed = value;
  else if (strcmp(key, "mouse_sensitivity") == 0 || strcmp(key, "sensitivity") == 0) cam->mouse_sensitivity = value;
  else return luaL_error(L, "Unknown or read-only Camera property: %s", key);

  return 0;
}

extern "C" int l_getCamera(lua_State* L)
{
  Entity e = (Entity)luaL_checkinteger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");

  if (!gRegistry->has<Camera>(e)) {
    lua_pushnil(L);
    return 1;
  }
  
  lua_newtable(L);

  // Metatable 
  lua_newtable(L);

  // __index
  lua_pushinteger(L, (lua_Integer)e);
  lua_pushcclosure(L, camera_index, 1);
  lua_setfield(L, -2, "__index");

  // __newindex
  lua_pushinteger(L, (lua_Integer)e);
  lua_pushcclosure(L, camera_newindex, 1);
  lua_setfield(L, -2, "__newindex");

  lua_setmetatable(L, -2);

  return 1;
}



extern "C" int l_getEntity(lua_State* L)
{
  if (!gRegistry)
    return luaL_error(L, "Registry not set");

  const char* name = luaL_checkstring(L, 1);

  auto it = gRegistry->entities.find(name);

  if (it == gRegistry->entities.end()) {
    lua_pushnil(L);
    return 1;
  }

  lua_pushinteger(L, static_cast<lua_Integer>(it->second));
  return 1;
}




extern "C" int l_setVelocity(lua_State* L)
{
  if (!gRegistry)
    return luaL_error(L, "Registry not set");

  if (!gBodies)
    return luaL_error(L, "Physics not set");

  Entity e = (Entity)luaL_checkinteger(L, 1);
  float x = (float)luaL_checknumber(L, 2);
  float y = (float)luaL_checknumber(L, 3);
  float z = (float)luaL_checknumber(L, 4);

  if (!gRegistry->has<RigidBody>(e))
    return luaL_error(L, "Entity has no RigidBody");

  auto& body = gRegistry->get<RigidBody>(e);

  if (body.bodyID.IsInvalid())
    return luaL_error(L, "Physics body not created");

  if (gBodies->GetMotionType(body.bodyID) != JPH::EMotionType::Kinematic)
    return luaL_error(L, "Body is not kinematic");

  gBodies->SetLinearVelocity(body.bodyID, JPH::Vec3(x, y, z));

  return 0;
}