#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "engine.h"
#include "ShaderObject.h"
#include "CameraSystem.h"
#include "InputSystem.h"
#include "InputState.h"
#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "camera.h"
#include "settings.h"
#include "utils.h"
#include "registry.h"
#include "transform.h"
#include "shader.h"
#include "script.h"
#include "MeshLoader.h"
#include "entitylink.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

struct Engine {

	float delta_time = 0.0f;
	float last_frame = 0.0f;

	Registry registry;
	InputSystem input;
	InputState input_state;
  CameraSystem camera;
  TransformSystem transform;
  RenderSystem render;
  ScriptSystem script;
  MeshLoader mesh_loader;

  GLFWwindow* window;
  double mouse_x, mouse_y;
  Entity camera_entity;

	void init();
	void run();
	void close();
};