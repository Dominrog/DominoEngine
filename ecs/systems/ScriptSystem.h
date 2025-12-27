#pragma once

#include <iostream>

#include "registry.h"
#include "script.h"
#include "LuaBindings.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

struct ScriptSystem 
{
	void start(Registry& registry);
	void update(Registry& registry, float dt);
};