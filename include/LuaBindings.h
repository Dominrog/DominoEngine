#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "registry.h"
#include "transform.h"
#include "camera.h"
#include "InputState.h"
#include "entitylink.h"

void lua_setRegistry(Registry* registry);
void lua_setInput(InputState* input);
static int keyFromString(const char* key);
extern "C" int l_input_isDown(lua_State* L);
extern "C" int l_input_mouseDX(lua_State* L);
extern "C" int l_input_mouseDY(lua_State* L);

extern "C" int transform_index(lua_State* L);
extern "C" int transform_newindex(lua_State* L);
extern "C" int l_getTransform(lua_State* L);

extern "C" int camera_index(lua_State* L);
extern "C" int camera_newindex(lua_State* L);
extern "C" int l_getCamera(lua_State* L);

extern "C" int entitylink_index(lua_State* L);
extern "C" int entitylink_newindex(lua_State* L);
extern "C" int l_getEntityLink(lua_State* L);