#include "ScriptSystem.h"

void ScriptSystem::setRegistry(Registry* registry)
{
    lua_setRegistry(registry);
}

void ScriptSystem::start(Registry& registry)
{
  for (auto e : registry.view<Script>())
  {
    auto& sc = registry.get<Script>(e);

    if (sc.lua_file.empty())  continue;

    sc.lua_state = luaL_newstate();
    luaL_openlibs(sc.lua_state);

    lua_register(sc.lua_state, "getTransform", l_getTransform);

    lua_getglobal(sc.lua_state, "entity");
    lua_pushinteger(sc.lua_state, (int)e);  
    lua_getglobal(sc.lua_state, "getTransform"); 
    lua_pushinteger(sc.lua_state, (int)e);  
    lua_call(sc.lua_state, 1, 1);
    lua_setglobal(sc.lua_state, "transform"); 

    if (luaL_dofile(sc.lua_state, sc.lua_file.c_str()) != LUA_OK) {
      printf("Lua load error (%s): %s\n", sc.lua_file.c_str(), lua_tostring(sc.lua_state, -1));
      continue;
    }

    lua_getglobal(sc.lua_state, "onStart");
    if (lua_isfunction(sc.lua_state, -1))
      sc.onStartRef = luaL_ref(sc.lua_state, LUA_REGISTRYINDEX);
    else
      lua_pop(sc.lua_state, 1);

    lua_getglobal(sc.lua_state, "onUpdate");
    if (lua_isfunction(sc.lua_state, -1))
      sc.onUpdateRef = luaL_ref(sc.lua_state, LUA_REGISTRYINDEX);
    else
      lua_pop(sc.lua_state, 1);

    if (sc.onStartRef != LUA_NOREF) {
      lua_rawgeti(sc.lua_state, LUA_REGISTRYINDEX, sc.onStartRef);
      lua_pushinteger(sc.lua_state, (int)e);

      if (lua_pcall(sc.lua_state, 1, 0, 0) != LUA_OK) {
        printf("Lua onStart error: %s\n", lua_tostring(sc.lua_state, -1));
      }
    }
  }
}


void ScriptSystem::update(Registry& registry, float dt)
{
	for (auto e : registry.view<Script>())
  {
    auto& sc = registry.get<Script>(e);
    if (sc.onUpdateRef == LUA_NOREF) continue;

    lua_rawgeti(sc.lua_state, LUA_REGISTRYINDEX, sc.onUpdateRef);
    lua_pushnumber(sc.lua_state, dt);

    if (lua_pcall(sc.lua_state, 1, 0, 0) != LUA_OK) {
      printf("Lua onUpdate error: %s\n", lua_tostring(sc.lua_state, -1));
    }
  }
}