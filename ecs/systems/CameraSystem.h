#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "registry.h"
#include "camera.h"
#include "transform.h"
#include "settings.h"

struct CameraSystem 
{
  void update(Registry& registry);
};