#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#include "registry.h"
#include "transform.h"
#include "boxcollider.h"
#include "physics.h"

struct PhysicsSystem {
	void update(Registry& registry, float delta_time);
};