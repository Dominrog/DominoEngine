#include "engine.h"

void Engine::init()
{
	glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Domino Engine", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, &input_state);
  setInputState(&input_state);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
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

}

void Engine::run()
{
	//----------------------START------------------------

  script.setRegistry(&registry);
  script.setInputState(&input_state);
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

    input.update(input_state, registry, delta_time, window);
    camera.update(registry);
    transform.update(registry);
    render.update(registry, camera_entity);
    script.update(registry, delta_time);

    //----------------------RESET-MOUSE-------------------

    input_state.mouse_dx = 0.0f;
    input_state.mouse_dy = 0.0f;
    input_state.scroll = 0.0f;

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
}

void Engine::close()
{
	ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
}