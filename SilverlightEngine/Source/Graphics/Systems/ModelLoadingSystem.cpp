#include "ModelLoadingSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Graphics/Components/Mesh/CustomMeshComponent.h"
#include "Graphics/MeshData.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/GLMConfig.h"

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

namespace Silverlight
{
	static bool LoadImageDataCallback(tinygltf::Image* _image, const int _image_idx, std::string* _err, std::string* _warn, int _req_width, int _req_height, const unsigned char* _bytes, int _size, void* _user_data)
	{
		auto* data{ static_cast<std::pair<std::vector<uint16>*, std::unordered_map<uint16, uint16>*>*>(_user_data) };
		auto* textureIds{ data->first };
		auto* textureIdMap{ data->second };

		const uint16 textureId{ g_ResourceManager.LoadImageFromMemory(_bytes, _size) };
		textureIds->push_back(textureId);
		(*textureIdMap)[_image_idx] = textureId;

		return true;
	}

	ModelLoadingSystem::ModelLoadingSystem(CustomMeshComponent& _meshComponent, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices)
	{
		tinygltf::Model model{};
		tinygltf::TinyGLTF loader{};
		std::string err{};
		std::string warn{};
		std::pair<std::vector<uint16>*, std::unordered_map<uint16, uint16>*> data{ &m_TextureIds, &m_TextureIdMap };

		loader.SetImageLoader(LoadImageDataCallback, &data);

		const std::string path{ _meshComponent.GetModelPath() };
		const std::filesystem::path filePath{ path };
		const std::string extension{ filePath.extension().string() };

		bool success{ false };

		if (extension == ".glb") 
		{
			success = loader.LoadBinaryFromFile(&model, &err, &warn, path);
		}
		else if (extension == ".gltf") 
		{
			success = loader.LoadASCIIFromFile(&model, &err, &warn, path);
		}
		else 
		{
			SE_LOG(LogCategory::Error, "[MODEL LOADING SYSTEM]: Unsupported file extension: {}", extension.c_str());
			throw std::runtime_error("[ERROR]: Failed to load model");
		}

		LoadModel(model, _meshComponent, _vertices, _indices);
	}

	void ModelLoadingSystem::LoadModel(const tinygltf::Model& _model, CustomMeshComponent& _meshComponent, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices)
	{
		for (size_t i = 0; i < _model.nodes.size(); ++i)
		{
			const tinygltf::Node& node{ _model.nodes.at(i) };
			if (node.mesh < 0)
			{
				continue;
			}

			glm::mat4 nodeTransform{ 1.0f };
			GetNodeTransform(node, nodeTransform);

			const tinygltf::Mesh& mesh{ _model.meshes.at(node.mesh) };
			for (const auto& primitive : mesh.primitives)
			{
				const auto posIt{ primitive.attributes.find("POSITION") };

				if (posIt == primitive.attributes.end())
				{
					continue;
				}

				const auto& positionsAccessor{ _model.accessors.at(posIt->second) };
				const auto& positionsView{ _model.bufferViews.at(positionsAccessor.bufferView) };
				const auto& positionsBuffer{ _model.buffers.at(positionsView.buffer) };
				const float* positions{ reinterpret_cast<const float*>(&(positionsBuffer.data.at(positionsView.byteOffset + positionsAccessor.byteOffset))) };

				const size_t vertexCount{ positionsAccessor.count };
				if (vertexCount == 0)
				{
					continue;
				}

				std::vector<Vertex> meshVertices(vertexCount);

				const float* normals{ nullptr };
				const float* texCoords{ nullptr };

				if (auto normalIt = primitive.attributes.find("NORMAL"); normalIt != primitive.attributes.end())
				{
					const auto& normalAccessor{ _model.accessors.at(normalIt->second) };
					const auto& normalView{ _model.bufferViews.at(normalAccessor.bufferView) };
					const auto& normalBuffer{ _model.buffers.at(normalView.buffer) };
					normals = reinterpret_cast<const float*>(&(normalBuffer.data.at(normalView.byteOffset + normalAccessor.byteOffset)));
				}

				if (auto texIt = primitive.attributes.find("TEXCOORD_0"); texIt != primitive.attributes.end())
				{
					const auto& texAccessor{ _model.accessors.at(texIt->second) };
					const auto& texView{ _model.bufferViews.at(texAccessor.bufferView) };
					const auto& texBuffer{ _model.buffers.at(texView.buffer) };
					texCoords = reinterpret_cast<const float*>(&(texBuffer.data.at(texView.byteOffset + texAccessor.byteOffset)));
				}

				for (size_t j = 0; j < vertexCount; ++j)
				{
					Vertex& v{ meshVertices[j] };

					v.m_Position = glm::vec3
					(
						positions[j * 3 + 0],
						positions[j * 3 + 1],
						positions[j * 3 + 2]
					);

					v.m_Normal = normals ? glm::vec3
					(
						normals[j * 3 + 0],
						normals[j * 3 + 1],
						normals[j * 3 + 2]
					) : glm::vec3(0.0f, 1.0f, 0.0f); 

					v.m_TexCoord = texCoords ? glm::vec2
					(
						texCoords[j * 2 + 0],
						texCoords[j * 2 + 1]
					) : glm::vec2(0.0f);
				}

				Silverlight::MeshData meshData{};
				meshData.SetModelMatrix(nodeTransform);
				const uint32 vertexOffset{ static_cast<uint32>(_vertices.size()) };
				meshData.SetIndexOffset(static_cast<uint32>(_indices.size()));

				_vertices.insert(_vertices.end(), meshVertices.begin(), meshVertices.end());

				const auto& indicesAccessor{ _model.accessors.at(primitive.indices) };
				const auto& indicesView{ _model.bufferViews.at(indicesAccessor.bufferView) };
				const auto& indicesBuffer{ _model.buffers.at(indicesView.buffer) };

				const uint16* indices{ reinterpret_cast<const uint16*>(&(indicesBuffer.data.at(indicesView.byteOffset + indicesAccessor.byteOffset))) };
				std::vector<uint32> subMeshIndices(indicesAccessor.count);
				for (size_t j = 0; j < indicesAccessor.count; ++j)
				{
					subMeshIndices[j] = indices[j] + vertexOffset;
				}

				_indices.insert(_indices.end(), subMeshIndices.begin(), subMeshIndices.end());

				meshData.SetIndexCount(static_cast<uint32>(subMeshIndices.size()));
				LoadMaterial(_model, primitive, meshData);

				_meshComponent.AddMeshData(meshData);
			}
		}
	}

	void ModelLoadingSystem::GetNodeTransform(const tinygltf::Node& _node, glm::mat4& _outTransform) const
	{
		constexpr uint16 MATRIX_SIZE{ 16 };
		constexpr uint16 TRANSLATION_SIZE{ 3 };
		constexpr uint16 ROTATION_SIZE{ 4 };
		constexpr uint16 SCALE_SIZE{ 3 };

		if (_node.matrix.size() == MATRIX_SIZE)
		{
			// Node has a transformation matrix
			_outTransform = glm::make_mat4(_node.matrix.data());
		}
		else
		{
			// Node has TRS (Translation, Rotation, Scale)
			if (_node.translation.size() == TRANSLATION_SIZE)
			{
				_outTransform = glm::translate(_outTransform, glm::vec3(_node.translation.at(0), _node.translation.at(1), _node.translation.at(2)));
			}

			if (_node.rotation.size() == ROTATION_SIZE)
			{
				const glm::quat rotation{ glm::make_quat(_node.rotation.data()) };
				_outTransform *= glm::mat4_cast(rotation);
			}

			if (_node.scale.size() == SCALE_SIZE)
			{
				_outTransform = glm::scale(_outTransform, glm::vec3(_node.scale.at(0), _node.scale.at(1), _node.scale.at(2)));
			}
		}
	}

	void ModelLoadingSystem::LoadMaterial(const tinygltf::Model& _model, const tinygltf::Primitive& _primitive, MeshData& _meshData)
	{
		if (_primitive.material < 0)
		{
			return;
		}

		const auto& tinyMaterial{ _model.materials.at(_primitive.material) };
		if (tinyMaterial.values.find("baseColorFactor") != tinyMaterial.values.end())
		{
			const auto& colorFactor{ tinyMaterial.values.at("baseColorFactor").ColorFactor() };
			_meshData.SetDiffuseColor(glm::vec4(colorFactor.at(0), colorFactor.at(1), colorFactor.at(2), 1.0f));
		}

		if (tinyMaterial.values.find("baseColorTexture") != tinyMaterial.values.end())
		{
			const int tinyTextureIndex{ tinyMaterial.values.at("baseColorTexture").TextureIndex() };
			const uint16 textureId{ m_TextureIdMap.at(tinyTextureIndex) };
			_meshData.SetTexId(textureId);
		}
	}
} // End of namespace