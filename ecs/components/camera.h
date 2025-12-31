#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera {
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;

  float yaw = -90.0f;
  float pitch = 0.0f;
  float movement_speed = 5.0f;
  float mouse_sensitivity = 100.0f;
  float zoom = 45.0f;
};