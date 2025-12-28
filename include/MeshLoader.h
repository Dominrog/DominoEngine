#pragma once

#include "tiny_gltf.h"
#include "mesh.h"
#include "vertex.h"
#include <iostream>
#include <string>
#include <vector>

struct MeshLoader {
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string err, warn;

	const unsigned char* getAccessorData(const tinygltf::Model& model, const tinygltf::Accessor& accessor);
	void importVertices(const tinygltf::Model& model, const tinygltf::Primitive& prim, std::vector<Vertex>& vertices);
	void importIndices(const tinygltf::Model& model, const tinygltf::Primitive& prim, std::vector<unsigned int>& indices);

	Mesh importMesh(const std::string& file_path);
};