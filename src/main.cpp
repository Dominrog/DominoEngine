#include "engine.h"


int main()
{
  
  Engine domino_engine;
  domino_engine.init();


  //----------------------------------------------------------------------------------------------

  ShaderObject object_shader("../shaders/objectShader.vs", "../shaders/objectShader.fs");


  Entity mesh_entity = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(mesh_entity, Transform {
    .position = glm::vec3(0.0f, 0.0f, 0.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f)
  });

  domino_engine.registry.addComponent<Mesh>(mesh_entity, domino_engine.mesh_loader.importMesh("../resources/Monkey.glb"));

  domino_engine.registry.addComponent<Shader>(mesh_entity, Shader {
    .shaderObj = &object_shader
  });

  domino_engine.registry.addComponent<Script>(mesh_entity, Script {
    .lua_file = "../ecs/scripts/test.lua"
  }); 



  domino_engine.camera_entity = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(domino_engine.camera_entity, Transform {
    .position = glm::vec3(0.0f, 1.0f, 3.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f)
  });

  domino_engine.registry.addComponent<Camera>(domino_engine.camera_entity, Camera{
    .front = glm::vec3(0.0f, 0.0f, -1.0f),
    .up = glm::vec3(0.0f, 1.0f, 0.0f),
    .right = glm::vec3(1.0f, 0.0f, 0.0f),
    .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
    .yaw = -90.0f,
    .pitch = -15.0f,
    .movement_speed = 5.0f,
    .mouse_sensitivity = 0.1f,
    .zoom = 45.0f
  });

  domino_engine.registry.addComponent<Script>(domino_engine.camera_entity, Script {
    .lua_file = "../ecs/scripts/CameraController.lua"
  }); 

  domino_engine.registry.addComponent<EntityLink>(domino_engine.camera_entity, EntityLink {
    .target = mesh_entity
  }); 


  Entity plane = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(plane, Transform {
    .position = glm::vec3(0.0f, -3.0f, 0.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(10.0f)
  });

  domino_engine.registry.addComponent<Mesh>(plane, domino_engine.mesh_loader.importMesh("../resources/Plane.glb"));

  domino_engine.registry.addComponent<Shader>(plane, Shader {
    .shaderObj = &object_shader
  });

  //----------------------------------------------------------------------------------------------


  domino_engine.run();
  domino_engine.close();

  return 0;
}
