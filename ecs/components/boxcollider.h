#pragma once

#include <glm/glm.hpp>

struct BoxCollider {
	glm::vec3 scale;
	glm::vec3 offset;
	uint32_t layer;
	uint32_t mask;
	bool trigger;
};