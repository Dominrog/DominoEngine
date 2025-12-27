#pragma once

#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "registry.h"
#include "transform.h"

extern Registry registry;

extern "C" int transform_index(lua_State* L);
extern "C" int transform_newindex(lua_State* L);
extern "C" int l_getTransform(lua_State* L);