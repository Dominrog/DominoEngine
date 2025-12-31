#pragma once

#include "registry.h"
#include "utils.h"
#include "InputState.h"

struct RawInput 
{
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float scroll_offset = 0.0f;
};


struct InputSystem 
{
	void update(InputState& input, Registry& registry, float dt, GLFWwindow* window);
};