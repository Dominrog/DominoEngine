#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#include "registry.h"
#include "transform.h"
#include "boxcollider.h"

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

struct OBB {
	glm::vec3 center;
	glm::vec3 axes[3];
	glm::vec3 scale;
};

struct CollisionSystem {
	void update(Registry& registry);
	bool aabbOverlap(const AABB& a, const AABB& b);
	AABB fittingAABB(const OBB& obb);
	bool obbOverlap(const OBB& a, const OBB& b);
	bool overlapOnAxis(const glm::vec3& axis, const OBB& a, const OBB& b);
};