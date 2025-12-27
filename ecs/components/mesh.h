#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "vertex.h"

struct Mesh 
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	unsigned int index_count = 0;

	bool is_imported = false;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};