#pragma once

#include "registry.h"
#include "transform.h"

class Behaviour
{
public:
	virtual ~Behaviour() = default;
	virtual void onStart(Entity entity, Registry& registry) = 0;
	virtual void onUpdate(Entity entity, Registry& registry, float deltaTime) = 0;
};