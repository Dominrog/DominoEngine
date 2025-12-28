#include "MeshLoader.h"

const unsigned char* MeshLoader::getAccessorData(const tinygltf::Model& model, const tinygltf::Accessor& accessor)
{
  const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];

  const tinygltf::Buffer& buffer = model.buffers[view.buffer];

  return buffer.data.data() + view.byteOffset + accessor.byteOffset;
}

void MeshLoader::importVertices(const tinygltf::Model& model, const tinygltf::Primitive& prim, std::vector<Vertex>& vertices)
{
  // POSITION
  const tinygltf::Accessor& posAcc = model.accessors[prim.attributes.at("POSITION")];

  const float* positions = reinterpret_cast<const float*>(getAccessorData(model, posAcc));

  size_t vertexCount = posAcc.count;
  vertices.resize(vertexCount);

  // NORMAL
  const float* normals = nullptr;
  if (prim.attributes.contains("NORMAL")) {
    const auto& normAcc = model.accessors[prim.attributes.at("NORMAL")];
    normals = reinterpret_cast<const float*>(getAccessorData(model, normAcc));
  }

  // TEXCOORD
  const float* uvs = nullptr;
  if (prim.attributes.contains("TEXCOORD_0")) {
    const auto& uvAcc = model.accessors[prim.attributes.at("TEXCOORD_0")];
    uvs = reinterpret_cast<const float*>(getAccessorData(model, uvAcc));
  }

  for (size_t i = 0; i < vertexCount; ++i) {
    Vertex& v = vertices[i];

    v.pos = {
      positions[i * 3 + 0],
      positions[i * 3 + 1],
      positions[i * 3 + 2]
    };

    v.normal = normals
      ? glm::vec3(
	        normals[i * 3 + 0],
	        normals[i * 3 + 1],
	        normals[i * 3 + 2])
      : glm::vec3(0.0f);

    v.uv = uvs
      ? glm::vec2(
          uvs[i * 2 + 0],
          uvs[i * 2 + 1])
      : glm::vec2(0.0f);
  }
}

void MeshLoader::importIndices(const tinygltf::Model& model, const tinygltf::Primitive& prim, std::vector<unsigned int>& indices)
{
  if (prim.indices < 0)
    return;

  const tinygltf::Accessor& acc =
    model.accessors[prim.indices];

  const unsigned char* data =
    getAccessorData(model, acc);

  indices.resize(acc.count);

  if (acc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
    const uint16_t* src = reinterpret_cast<const uint16_t*>(data);
    for (size_t i = 0; i < acc.count; ++i)
      indices[i] = src[i];
  }
  else if (acc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
    const uint32_t* src = reinterpret_cast<const uint32_t*>(data);
    for (size_t i = 0; i < acc.count; ++i)
      indices[i] = src[i];
  }
}

Mesh MeshLoader::importMesh(const std::string& path)
{
  Mesh mesh;

  if (!loader.LoadBinaryFromFile(&model, &err, &warn, path))
    return mesh;

  int sceneIndex = model.defaultScene >= 0 ? model.defaultScene : 0;
  const auto& scene = model.scenes[sceneIndex];

  for (int nodeIndex : scene.nodes) {
    const auto& node = model.nodes[nodeIndex];
    if (node.mesh < 0) continue;

    const auto& gltfMesh = model.meshes[node.mesh];
    const auto& prim = gltfMesh.primitives[0];

    importVertices(model, prim, mesh.vertices);
    importIndices(model, prim, mesh.indices);

    mesh.index_count = static_cast<unsigned int>(mesh.indices.size());
    mesh.is_imported = true;
    break;
  }

  return mesh;
}
