#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#include "registry.h"
#include "transform.h"
#include "boxcollider.h"
#include "physics.h"

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

struct OBB {
	glm::vec3 center;
	glm::vec3 axes[3];
	glm::vec3 scale;
};

struct CollisionInfo {
  glm::vec3 normal;
  glm::vec3 point;
  float depth;
  bool hasCollision;
};

struct CollisionSystem {
	void update(Registry& registry, float delta_time);
	bool aabbOverlap(const AABB& a, const AABB& b);
	AABB fittingAABB(const OBB& obb);
	bool obbOverlap(const OBB& a, const OBB& b);
	bool overlapOnAxis(const glm::vec3& axis, const OBB& a, const OBB& b);
	CollisionInfo getCollisionInfo(const OBB& a, const OBB& b);
};