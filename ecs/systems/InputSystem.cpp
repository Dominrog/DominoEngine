#include "InputSystem.h"

void InputSystem::update(Registry& registry, float dt, GLFWwindow* window)
{
	for (auto e : registry.view<Camera, Transform>())
  {
    auto& cam = registry.get<Camera>(e);
    auto& tr = registry.get<Transform>(e);

    float velocity = cam.movement_speed * dt;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);

	  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	    tr.position += cam.front * velocity;
	  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	    tr.position -= cam.front * velocity;
	  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	    tr.position -= cam.right * velocity;
	  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	    tr.position += cam.right * velocity;


	  cam.zoom -= gInput.scroll_offset;
    cam.zoom = glm::clamp(cam.zoom, 1.0f, 45.0f);


	  cam.yaw   += gInput.xoffset  * cam.mouse_sensitivity;
		cam.pitch += gInput.yoffset  * cam.mouse_sensitivity;

	  cam.pitch = glm::clamp(cam.pitch, -89.0f, 89.0f);
  }

  gInput.xoffset = 0;
  gInput.yoffset = 0;
  gInput.scroll_offset = 0;
}