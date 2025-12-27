#pragma once

#include "registry.h"
#include "utils.h"

struct RawInput 
{
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float scroll_offset = 0.0f;
};


struct InputSystem 
{
	void update(Registry& registry, float dt, GLFWwindow* window);
};