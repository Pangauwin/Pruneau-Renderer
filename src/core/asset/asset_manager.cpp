#include "asset_manager.h"

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/application.h"

#include "renderer/texture.h"

static Core::AssetID nextID = 1;

namespace Core {
	std::unordered_map<AssetID, std::shared_ptr<Asset>> AssetManager::m_assets;
}

/*
TODO: Create import failed states and warnings on import
*/

template<typename T>
std::shared_ptr<T> Core::AssetManager::GetAsset(AssetID id)
{
	auto it = m_assets.find(id);

	if (it == m_assets.end())
		return nullptr;

	return std::dynamic_pointer_cast<T>(it->second);
}

Core::AssetID Core::AssetManager::ImportAsset(const std::string& path)
{
	if (EndsWith(path, ".obj") || EndsWith(path, ".fbx"))
	{
		ImportModel(std::move(path));
	}

	nextID++;

	std::shared_ptr<Asset> _asset;
	//TODO : assign the new asset inside _asset

	return nextID;
}

void Core::AssetManager::ImportModel(std::string _path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	std::vector<std::weak_ptr<Core::Asset>> _meshes;

	ProcessNode(scene->mRootNode, scene, _meshes);

	//TODO #1 : apply the right shader to the model
	//TODO #2 : apply the right transform of the meshes (if that exists ?) inside the model
	//TODO #3 : when all of that is done, create a model asset
}

void Core::AssetManager::ProcessNode(void* _node, const void* _scene, std::vector<std::weak_ptr<Core::Asset>>& _meshes)
{
	aiNode* node = (aiNode*)_node;
	const aiScene* scene = (aiScene*)_scene;

	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, _meshes);
	}
}

std::weak_ptr<Core::Asset> Core::AssetManager::ProcessMesh(void* _mesh, const void* _scene)
{
	aiMesh* mesh = (aiMesh*)_mesh;
	const aiScene* scene = (aiScene*)_scene;

	std::vector<Renderer::Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Renderer::Vertex vertex;
		vertex.position = glm::vec3({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		vertex.normal = glm::vec3({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0])
			vertex.uv_coord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.uv_coord = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// TODO : make this rework again (texture assigned to material on import) AND texture import automatically
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(material, (void*)aiTextureType_DIFFUSE, Renderer::TEXTURE_TYPE_DIFFUSE);
		LoadMaterialTextures(material, (void*)aiTextureType_SPECULAR, Renderer::TEXTURE_TYPE_NORMAL_MAP);
	}

	nextID++;
	Core::AssetManager::m_assets[nextID] = std::make_shared<MeshAsset>("Mesh_" + std::to_string(nextID), nextID, vertices, indices);

	return Core::AssetManager::m_assets[nextID];
}

static void LoadMaterialTextures(void* _mat, void* _type, Renderer::TEXTURE_TYPE _type_name)
{
	aiMaterial* mat = (aiMaterial*)_mat;
	aiTextureType type = *(aiTextureType*)_type;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString _path;
		mat->GetTexture(type, i, &_path);

		Core::AssetManager::ImportAsset(_path.C_Str());
	}
}

static bool EndsWith(const std::string& value, const std::string& ending)
{
	if (ending.size() > value.size())
		return false;

	return std::equal(
		ending.rbegin(), ending.rend(),
		value.rbegin()
	);
}