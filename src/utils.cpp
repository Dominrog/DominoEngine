#include "utils.h"

int global_fps = 0;

static InputState* gInput = nullptr;

void setInputState(InputState* input) {
  gInput = input;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (gInput->first_mouse)
  {
    gInput->mouse_x = xpos;
    gInput->mouse_y = ypos;
    gInput->first_mouse = false;
    return;
  }

  gInput->mouse_dx = xpos - gInput->mouse_x;
  gInput->mouse_dy = gInput->mouse_y - ypos;

  gInput->mouse_x = xpos;
  gInput->mouse_y = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  gInput->scroll += static_cast<float>(yoffset);
}

void countFps()
{
  static double lastTime = glfwGetTime();
  static int frameCount = 0;

  double currentTime = glfwGetTime();
  frameCount++;

  if (currentTime - lastTime >= 1.0)
  {
    global_fps = frameCount;
    frameCount = 0;
    lastTime = currentTime;
  }
}
