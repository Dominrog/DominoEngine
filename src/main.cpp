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

#include "ShaderObject.h"
#include "CameraSystem.h"
#include "InputSystem.h"
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

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


float delta_time = 0.0f;
float last_frame = 0.0f;


std::vector<Vertex> vertices = 
{
  {{-0.5f,  0.0f, -0.5f}, {0,0,1}, {0,0}},
  {{ 0.5f,  0.0f, -0.5f}, {0,0,1}, {1,0}},
  {{ 0.5f,  0.0f,  0.5f}, {0,0,1}, {1,1}},
  {{-0.5f,  0.0f,  0.5f}, {0,0,1}, {0,1}}
};

std::vector<unsigned int> indices = 
{
  0,2,1,
  0,3,2
};


Registry registry;


int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Domino Engine", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);  
  glfwSwapInterval(0);

  double mouse_x, mouse_y;


  ShaderObject object_shader("../shaders/objectShader.vs", "../shaders/objectShader.fs");

  //----------------------INITIALISE-SYSTEMS------------------------

  InputSystem input;
  CameraSystem camera;
  TransformSystem transform;
  RenderSystem render;
  ScriptSystem script;

  MeshLoader mesh_loader;

  //----------------------------------------------------------------


  Entity camera_entity = registry.createEntity();

  registry.addComponent<Transform>(camera_entity, Transform {
    .position = glm::vec3(0.0f, 3.0f, 0.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f)
  });

  registry.addComponent<Camera>(camera_entity, Camera{
    .front = glm::vec3(0.0f, 0.0f, -1.0f),
    .up = glm::vec3(0.0f, 1.0f, 0.0f),
    .right = glm::vec3(1.0f, 0.0f, 0.0f),
    .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
    .yaw = -90.0f,
    .pitch = 0.0f,
    .movement_speed = 5.0f,
    .mouse_sensitivity = 0.1f,
    .zoom = 45.0f
  });


  Entity mesh_entity = registry.createEntity();

  registry.addComponent<Transform>(mesh_entity, Transform {
    .position = glm::vec3(0.0f, 0.0f, 0.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f)
  });

  registry.addComponent<Mesh>(mesh_entity, mesh_loader.importMesh("../resources/Monkey.glb"));

  registry.addComponent<Shader>(mesh_entity, Shader {
    .shaderObj = &object_shader
  });

  
  registry.addComponent<Script>(mesh_entity, Script {
    .lua_file = "../ecs/scripts/test.lua"
  }); 

  //----------------------START------------------------

  script.start(registry);

  //----------------------------------------------------


  while(!glfwWindowShouldClose(window))
  {
    float current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    countFps();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glfwGetCursorPos(window, &mouse_x, &mouse_y);


    //----------------------UPDATE------------------------

    input.update(registry, delta_time, window);
    camera.update(registry);
    transform.update(registry);
    render.update(registry, camera_entity);
    script.update(registry, delta_time);

    //----------------------------------------------------


    ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_Once);
    ImGui::Begin("Engine Info");

    ImGui::GetIO().FontGlobalScale = 3.0f;
    ImGui::Text("FPS: %i", global_fps);

    ImGui::Text("X: %f", registry.get<Transform>(camera_entity).position.x);
    ImGui::Text("Y: %f", registry.get<Transform>(camera_entity).position.y);
    ImGui::Text("Z: %f", registry.get<Transform>(camera_entity).position.z);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
