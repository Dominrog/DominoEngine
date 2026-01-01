#include "InputSystem.h"

void InputSystem::update(InputState& input, Registry& registry, float dt, GLFWwindow* window)
{

	for (int key = 0; key < GLFW_KEY_LAST; ++key)
  {
  	input.keys[key] = glfwGetKey(window, key) == GLFW_PRESS;
  }

}