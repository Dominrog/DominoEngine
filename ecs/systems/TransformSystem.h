#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "registry.h"
#include "transform.h"

struct TransformSystem
{
	void update(Registry& registry);
};