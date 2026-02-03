#pragma once

#include <glm/glm.hpp>

struct Physics {
	float mass = 1.0f;
	float invMass = 0.0f;
	float restitution = 0.3;

	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 forceAccum = glm::vec3(0.0f);
	glm::vec3 gravity = glm::vec3(0.0f, 9.81f, 0.0f);
};