#include "PhysicsSystem.h"

void PhysicsSystem::update(Registry& registry, float delta_time)
{
	auto view = registry.view<Physics>();

	for (auto e : view)
	{
		auto& p = registry.get<Physics>(e);

		if (p.mass > 0) p.invMass = 1.0f/p.mass;

		p.forceAccum += p.mass * -p.gravity;
	}
}