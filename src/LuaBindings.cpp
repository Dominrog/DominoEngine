#include "LuaBindings.h"

static Registry* gRegistry = nullptr;

void lua_setRegistry(Registry* registry)
{
  gRegistry = registry;
}

extern "C" int transform_index(lua_State* L) {
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

extern "C" int transform_newindex(lua_State* L) {
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

extern "C" int l_getTransform(lua_State* L) {
  Entity e = (Entity)lua_tointeger(L, 1);

  if (!gRegistry)
    return luaL_error(L, "Registry not set");
  
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

