#include "engine.h"


int main()
{
  Engine domino_engine;
  domino_engine.init();

  //----------------------------------------------------------------------------------------------

  ShaderObject object_shader("../shaders/objectShader.vs", "../shaders/objectShader.fs");


  Entity ball = domino_engine.registry.createEntity("ball");

  domino_engine.registry.addComponent<Mesh>(ball, domino_engine.mesh_loader.importMesh("../resources/Cube.glb"));

  domino_engine.registry.addComponent<Shader>(ball, Shader {
    .shaderObj = &object_shader
  });

  domino_engine.registry.addComponent<Script>(ball, Script {
    .lua_file = "../ecs/scripts/ball.lua"
  });

  Transform ballTransform;
  ballTransform.position = glm::vec3(0.0f, 1.5f, -5.0f);
  ballTransform.scale = glm::vec3(0.25f, 0.25f, 0.25f);

  Collider ballCollider;
  ballCollider.shape =
      new JPH::BoxShape(JPH::Vec3(0.25f, 0.25f, 0.25f));

  RigidBody ballBody;
  ballBody.motionType = JPH::EMotionType::Dynamic; // Dynamic reagiert auf gravity

  domino_engine.registry.addComponent<Transform>(ball, ballTransform);
  domino_engine.registry.addComponent<Collider>(ball, ballCollider);
  domino_engine.registry.addComponent<RigidBody>(ball, ballBody);

  domino_engine.physics.createBody(domino_engine.registry, ball);



  domino_engine.camera_entity = domino_engine.registry.createEntity("camera");

  domino_engine.registry.addComponent<Transform>(domino_engine.camera_entity, Transform {
    .position = glm::vec3(0.0f, 0.0f, 5.0f),
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


  Entity platform = domino_engine.registry.createEntity("platform");

  domino_engine.registry.addComponent<Mesh>(platform, domino_engine.mesh_loader.importMesh("../resources/Cube.glb"));

  domino_engine.registry.addComponent<Script>(platform, Script {
    .lua_file = "../ecs/scripts/PlatformController.lua"
  });

  domino_engine.registry.addComponent<Shader>(platform, Shader {
    .shaderObj = &object_shader
  });

  Transform platformTransform;
  platformTransform.position = glm::vec3(0.0f, -2.0f, -5.0f);
  platformTransform.scale = glm::vec3(0.25f, 5.0f, 1.0f);

  Collider platformCollider;
  platformCollider.shape =
      new JPH::BoxShape(JPH::Vec3(0.25f, 5.0f, 1.0f));

  RigidBody platformBody;
  platformBody.motionType = JPH::EMotionType::Kinematic;

  domino_engine.registry.addComponent<Transform>(platform, platformTransform);
  domino_engine.registry.addComponent<Collider>(platform, platformCollider);
  domino_engine.registry.addComponent<RigidBody>(platform, platformBody);

  domino_engine.physics.createBody(domino_engine.registry, platform);

  //----------------------------------------------------------------------------------------------

  print_entity_list(&domino_engine);

  domino_engine.run();
  domino_engine.close();

  return 0;
}
