#include "TransformSystem.h"

void TransformSystem::update(Registry& registry)
{
	for(auto e : registry.view<Transform>())
	{
		auto& t = registry.get<Transform>(e);

		glm::mat4 S = glm::scale(glm::mat4(1.0f), t.scale);

		glm::mat4 T = glm::translate(glm::mat4(1.0f), t.position);

    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(t.rotation.x), glm::vec3(1,0,0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(t.rotation.y), glm::vec3(0,1,0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(t.rotation.z), glm::vec3(0,0,1));
    glm::mat4 R = Rz * Ry * Rx;

    t.model_matrix = T * S * R;
	}
}