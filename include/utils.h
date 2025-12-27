#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "CameraSystem.h"
#include "settings.h"
#include "InputSystem.h"

struct RawInput;

extern bool firstMouse;
extern int global_fps;
extern float last_x;
extern float last_y;

extern RawInput gInput;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void countFps();
//glm::vec3 getMouseRay(double mouseX, double mouseY, const CameraSystem& camera);
//glm::vec3 intersectRayPlane(glm::vec3 rayOrigin, glm::vec3 rayDir, float planeY);