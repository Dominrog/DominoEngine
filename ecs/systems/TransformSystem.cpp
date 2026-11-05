#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

void TransformSystem::update(Registry& registry, float delta_time)
{
    for (Entity entity : registry.view<Transform>())
    {
        auto& t = registry.get<Transform>(entity);

        glm::mat4 T = glm::translate(
            glm::mat4(1.0f),
            t.position
        );

        glm::mat4 R = glm::mat4_cast(
            glm::normalize(t.rotationQ)
        );

        glm::mat4 S = glm::scale(
            glm::mat4(1.0f),
            t.scale
        );

        t.model_matrix = T * R * S;
    }
}