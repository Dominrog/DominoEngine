#include "RenderSystem.h"


void RenderSystem::update(Registry& registry, Entity camera_entity)
{
	const auto& camera = registry.get<Camera>(camera_entity);
	for (auto e : registry.view<Mesh, Transform, Shader>())
  {
  	auto& mesh = registry.get<Mesh>(e);
  	auto& transform = registry.get<Transform>(e);
    auto& shader = registry.get<Shader>(e);

  	if (mesh.VAO == 0)	upload(mesh);

  	shader.shaderObj->use();
    shader.shaderObj->setMat4("model", transform.model_matrix);
    shader.shaderObj->setMat4("view", camera.view_matrix);
    shader.shaderObj->setMat4("proj", camera.projection_matrix);

  	render(mesh);
  }
}

void RenderSystem::upload(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER,
               mesh.vertices.size()*sizeof(Vertex),
               mesh.vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh.indices.size()*sizeof(unsigned int),
               mesh.indices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

  mesh.index_count = mesh.indices.size();
}

void RenderSystem::render(Mesh& mesh)
{
	if (mesh.index_count == 0) return;

	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
