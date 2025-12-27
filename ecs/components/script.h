#pragma once

#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

struct Script
{
	std::string lua_file;
	lua_State* lua_state = nullptr;
	int onStartRef = LUA_NOREF;
	int onUpdateRef = LUA_NOREF;
};