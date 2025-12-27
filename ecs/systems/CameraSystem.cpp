#include "CameraSystem.h"


void CameraSystem::update(Registry& registry)
{
  for (auto e : registry.view<Camera, Transform>())
  {
    auto& cam = registry.get<Camera>(e);
    auto& tr = registry.get<Transform>(e);

    glm::vec3 front;
    front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    front.y = sin(glm::radians(cam.pitch));
    front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.front = glm::normalize(front);

    cam.right = glm::normalize(glm::cross(cam.front, cam.world_up)); 
    cam.up    = glm::normalize(glm::cross(cam.right, cam.front));

    cam.view_matrix = glm::lookAt(tr.position, tr.position + cam.front, cam.up);

    cam.projection_matrix = glm::perspective(
      glm::radians(cam.zoom),                // FOV
      (float)SCR_WIDTH / (float)SCR_HEIGHT,  // Aspect
      0.1f,                                  // Near plane
      1000.0f                                // Far plane
    );
  }
}