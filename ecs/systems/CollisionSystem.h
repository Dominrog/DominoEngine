#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "registry.h"
#include "transform.h"
#include "boxcollider.h"

struct CollisionSystem {
	void update(Registry& registry);
	bool aabbOverlap(const Transform& ta, const BoxCollider& ca, const Transform& tb, const BoxCollider& cb);
};