#include "LuaBindings.h"

static Registry* gRegistry = nullptr;
static InputState* gInput = nullptr;

void lua_setRegistry(Registry* registry)
{
  gRegistry = registry;
}

void lua_setInput(InputState* input)
{
  gInput = input;
}

static int keyFromString(const char* key)
{
  if (strcmp(key, "W") == 0) return GLFW_KEY_W;
  if (strcmp(key, "A") == 0) return GLFW_KEY_A;
  if (strcmp(key, "S") == 0) return GLFW_KEY_S;
  if (strcmp(key, "D") == 0) return GLFW_KEY_D;
  if (strcmp(key, "Space") == 0) return GLFW_KEY_SPACE;
  return -1;
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
  Transform* tr = (Transform*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);

  if (strcmp(key, "x") == 0) lua_pushnumber(L, tr->position.x);
  else if (strcmp(key, "y") == 0) lua_pushnumber(L, tr->position.y);
  else if (strcmp(key, "z") == 0) lua_pushnumber(L, tr->position.z);
  else if (strcmp(key, "rx") == 0) lua_pushnumber(L, tr->rotation.x);
  else if (strcmp(key, "ry") == 0) lua_pushnumber(L, tr->rotation.y);
  else if (strcmp(key, "rz") == 0) lua_pushnumber(L, tr->rotation.z);
  else if (strcmp(key, "sx") == 0) lua_pushnumber(L, tr->scale.x);
  else if (strcmp(key, "sy") == 0) lua_pushnumber(L, tr->scale.y);
  else if (strcmp(key, "sz") == 0) lua_pushnumber(L, tr->scale.z);
  else lua_pushnil(L);

  return 1;
}

extern "C" int transform_newindex(lua_State* L) 
{
  Transform* tr = (Transform*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);
  float value = (float)lua_tonumber(L, 3);

  if (strcmp(key, "x") == 0) tr->position.x = value;
  else if (strcmp(key, "y") == 0) tr->position.y = value;
  else if (strcmp(key, "z") == 0) tr->position.z = value;
  else if (strcmp(key, "rx") == 0) tr->rotation.x = value;
  else if (strcmp(key, "ry") == 0) tr->rotation.y = value;
  else if (strcmp(key, "rz") == 0) tr->rotation.z = value;
  else if (strcmp(key, "sx") == 0) tr->scale.x = value;
  else if (strcmp(key, "sy") == 0) tr->scale.y = value;
  else if (strcmp(key, "sz") == 0) tr->scale.z = value;

  return 0;
}

extern "C" int l_getTransform(lua_State* L) 
{
  Entity e = (Entity)lua_tointeger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");
  
  if (!gRegistry->has<Transform>(e))
    return 0;

  Transform* tr = &gRegistry->get<Transform>(e);
  
  lua_newtable(L);

  // Metatable 
  lua_newtable(L);

  // __index
  lua_pushlightuserdata(L, tr);
  lua_pushcclosure(L, transform_index, 1);
  lua_setfield(L, -2, "__index");

  // __newindex
  lua_pushlightuserdata(L, tr);
  lua_pushcclosure(L, transform_newindex, 1);
  lua_setfield(L, -2, "__newindex");

  lua_setmetatable(L, -2);

  return 1;
}

extern "C" int camera_index(lua_State* L)
{
  Camera* cam = (Camera*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);

  if (strcmp(key, "yaw") == 0) lua_pushnumber(L, cam->yaw);
  else if (strcmp(key, "pitch") == 0) lua_pushnumber(L, cam->pitch);
  else if (strcmp(key, "movement_speed") == 0) lua_pushnumber(L, cam->movement_speed);
  else if (strcmp(key, "mouse_sensitivity") == 0) lua_pushnumber(L, cam->mouse_sensitivity);
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
  Camera* cam = (Camera*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);
  float value = (float)lua_tonumber(L, 3);

  if (strcmp(key, "yaw") == 0) cam->yaw = value;
  else if (strcmp(key, "pitch") == 0) cam->pitch = glm::clamp(value, -89.0f, 89.0f);
  else if (strcmp(key, "zoom") == 0) cam->zoom = glm::clamp(value, 1.0f, 90.0f);
  else if (strcmp(key, "speed") == 0) cam->movement_speed = value;
  else if (strcmp(key, "sensitivity") == 0) cam->mouse_sensitivity = value;

  return 0;
}

extern "C" int l_getCamera(lua_State* L)
{
  Entity e = (Entity)lua_tointeger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");

  if (!gRegistry->has<Camera>(e)) 
    return 0;
  
  Camera* cam = &gRegistry->get<Camera>(e);
  
  lua_newtable(L);

  // Metatable 
  lua_newtable(L);

  // __index
  lua_pushlightuserdata(L, cam);
  lua_pushcclosure(L, camera_index, 1);
  lua_setfield(L, -2, "__index");

  // __newindex
  lua_pushlightuserdata(L, cam);
  lua_pushcclosure(L, camera_newindex, 1);
  lua_setfield(L, -2, "__newindex");

  lua_setmetatable(L, -2);

  return 1;
}

extern "C" int entitylink_index(lua_State* L)
{
  EntityLink* link = (EntityLink*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);

  if (strcmp(key, "target") == 0) lua_pushnumber(L, link->target);
  else lua_pushnil(L);

  return 1;
}

extern "C" int entitylink_newindex(lua_State* L)
{
  EntityLink* link = (EntityLink*)lua_touserdata(L, lua_upvalueindex(1));
  const char* key = lua_tostring(L, 2);
  float value = (float)lua_tonumber(L, 3);

  if (strcmp(key, "target") == 0) link->target = value;

  return 0;
}

extern "C" int l_getEntityLink(lua_State* L)
{
  Entity e = (Entity)lua_tointeger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");

  if (!gRegistry->has<EntityLink>(e)) 
    return 0;
  
  EntityLink* link = &gRegistry->get<EntityLink>(e);
  
  lua_newtable(L);

  // Metatable 
  lua_newtable(L);

  // __index
  lua_pushlightuserdata(L, link);
  lua_pushcclosure(L, entitylink_index, 1);
  lua_setfield(L, -2, "__index");

  // __newindex
  lua_pushlightuserdata(L, link);
  lua_pushcclosure(L, entitylink_newindex, 1);
  lua_setfield(L, -2, "__newindex");

  lua_setmetatable(L, -2);

  return 1;
}