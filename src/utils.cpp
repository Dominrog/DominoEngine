#include "utils.h"

bool first_mouse = true;

int global_fps = 0;

float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;

RawInput gInput;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (first_mouse)
  {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  gInput.xoffset = xpos - last_x;
  gInput.yoffset = last_y - ypos;

  last_x = xpos;
  last_y = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  gInput.scroll_offset = static_cast<float>(yoffset);
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
