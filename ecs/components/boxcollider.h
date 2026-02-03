#pragma once

#include <glm/glm.hpp>

struct BoxCollider {
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
	uint32_t layer = 1;
	uint32_t mask = 0xFFFFFFFF;
	bool trigger = true;
	bool colliding = false;
	bool collidable = true;
};