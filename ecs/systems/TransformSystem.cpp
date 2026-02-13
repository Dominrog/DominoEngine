#include "TransformSystem.h"

void TransformSystem::update(Registry& registry, float delta_time)
{
	for(auto e : registry.view<Transform, Physics>())
	{
		auto& t = registry.get<Transform>(e);
		auto& p = registry.get<Physics>(e);

		p.velocity += p.invMass * p.forceAccum * delta_time;

		t.position += p.velocity * delta_time;
		p.forceAccum = glm::vec3(0.0f);
	}

	for(auto e : registry.view<Transform>())
	{
		auto& t = registry.get<Transform>(e);

		glm::quat qx = glm::angleAxis(glm::radians(t.rotation.x), glm::vec3(1,0,0));
		glm::quat qy = glm::angleAxis(glm::radians(t.rotation.y), glm::vec3(0,1,0));
		glm::quat qz = glm::angleAxis(glm::radians(t.rotation.z), glm::vec3(0,0,1));

		t.rotationQ = qz * qy * qx;

		glm::mat4 S = glm::scale(glm::mat4(1.0f), t.scale);
    glm::mat4 R = glm::mat4_cast(glm::normalize(t.rotationQ));
    glm::mat4 T = glm::translate(glm::mat4(1.0f), t.position);

    t.model_matrix = T * R * S;
	}
}