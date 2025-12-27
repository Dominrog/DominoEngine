#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderObject.h"
#include "registry.h"
#include "transform.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"

struct RenderSystem
{
	void update(Registry& registry, Entity camera_entity);

	void upload(Mesh& mesh);
	void render(Mesh& mesh);
};