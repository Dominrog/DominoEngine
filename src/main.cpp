#include "engine.h"


int main()
{
  
  Engine domino_engine;
  domino_engine.init();


  //----------------------------------------------------------------------------------------------

  ShaderObject object_shader("../shaders/objectShader.vs", "../shaders/objectShader.fs");



  Entity mesh_entity = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(mesh_entity, Transform {
    .position = glm::vec3(0.0f, 0.0f, -5.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(0.25f)
  });

  domino_engine.registry.addComponent<Mesh>(mesh_entity, domino_engine.mesh_loader.importMesh("../resources/Cube.glb"));

  domino_engine.registry.addComponent<Shader>(mesh_entity, Shader {
    .shaderObj = &object_shader
  });

  domino_engine.registry.addComponent<Script>(mesh_entity, Script {
    .lua_file = "../ecs/scripts/ball.lua"
  }); 

  domino_engine.registry.addComponent<BoxCollider>(mesh_entity, BoxCollider{});



  domino_engine.camera_entity = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(domino_engine.camera_entity, Transform {
    .position = glm::vec3(0.0f, 0.0f, 10.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(1.0f)
  });

  domino_engine.registry.addComponent<Camera>(domino_engine.camera_entity, Camera{
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

  /*
  domino_engine.registry.addComponent<Script>(domino_engine.camera_entity, Script {
    .lua_file = "../ecs/scripts/CameraController.lua"
  });

  domino_engine.registry.addComponent<EntityLink>(domino_engine.camera_entity, EntityLink {
    .target = mesh_entity
  }); 
  */


  
  Entity left_pong = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(left_pong, Transform {
    .position = glm::vec3(-10.0f, 0.0f, -5.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(0.25f, 2.0f, 1.0f)
  });

  domino_engine.registry.addComponent<Mesh>(left_pong, domino_engine.mesh_loader.importMesh("../resources/Cube.glb"));

  domino_engine.registry.addComponent<Shader>(left_pong, Shader {
    .shaderObj = &object_shader
  });

  domino_engine.registry.addComponent<Script>(left_pong, Script {
    .lua_file = "../ecs/scripts/WallControllerLeft.lua"
  }); 

  domino_engine.registry.addComponent<BoxCollider>(left_pong, BoxCollider{});



  Entity right_pong = domino_engine.registry.createEntity();

  domino_engine.registry.addComponent<Transform>(right_pong, Transform {
    .position = glm::vec3(10.0f, 0.0f, -5.0f),
    .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
    .scale = glm::vec3(0.25f, 2.0f, 1.0f)
  });

  domino_engine.registry.addComponent<Mesh>(right_pong, domino_engine.mesh_loader.importMesh("../resources/Cube.glb"));

  domino_engine.registry.addComponent<Script>(right_pong, Script {
    .lua_file = "../ecs/scripts/WallControllerRight.lua"
  }); 

  domino_engine.registry.addComponent<Shader>(right_pong, Shader {
    .shaderObj = &object_shader
  });

  domino_engine.registry.addComponent<BoxCollider>(right_pong, BoxCollider{});


  //----------------------------------------------------------------------------------------------


  domino_engine.run();
  domino_engine.close();

  return 0;
}
