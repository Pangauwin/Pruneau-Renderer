#include "asset_manager.h"

#include <string>

#include "renderer/model.h"
#include "renderer/texture.h"
#include "renderer/shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/application.h"

Core::AssetManager* instance = nullptr;

/*
TODO: Create import failed states and warnings on import
*/

Core::AssetManager::AssetManager()
{
	if (instance)
		return;

	instance = this;
}

Core::Asset* Core::AssetManager::ImportAsset(const char* file_path)
{
	std::string path = file_path;

	std::string extension = path.substr(path.find_last_of(".") + 1);

	if (extension == "fbx")
	{
		std::string name = path;

		Asset* _asset = new Asset();
		_asset->name = name.erase(0, name.find_last_of("/") + 1).erase(0, name.find_last_of("\\") + 1).erase(name.find_last_of("."), name.length());
		_asset->type = ASSET_TYPE_MODEL;
		_asset->path = path;
		_asset->data = instance->CreateModelData(path);

		if (!_asset->data)
		{
			Core::LogMessage("Could not import asset: " + path);
			delete _asset;
			return nullptr;
		}

		Get()->m_assets.push_back(_asset);

		return _asset;
	}

	else if (extension == "png")
	{
		std::string name = path;

		Asset* _asset = new Asset();
		_asset->name = name.erase(0, name.find_last_of("/") + 1).erase(0, name.find_last_of("\\") + 1).erase(name.find_last_of("."), name.length());
		_asset->type = ASSET_TYPE_TEXTURE;
		_asset->path = path;

		_asset->data = instance->CreateTextureData(path);

		if (!_asset->data)
		{
			Core::LogMessage("Could not import asset: " + path);
			delete _asset;
			return nullptr;
		}

		Get()->m_assets.push_back(_asset);

		return _asset;
	}

	else if (extension == "glsl")
	{
		std::string name = path;

		Asset* _asset = new Asset();
		_asset->name = name.erase(0, name.find_last_of("/") + 1).erase(0, name.find_last_of("\\") + 1).erase(name.find_last_of("."), name.length());
		_asset->type = ASSET_TYPE_SHADER;
		_asset->path = path;

		_asset->data = instance->CreateTextureData(path);
		if (!_asset->data)
		{
			Core::LogMessage("Could not import asset: " + path);
			delete _asset;
			return nullptr;
		}

		Get()->m_assets.push_back(_asset);

		return _asset;
	}

	else
	{
		Core::LogMessage("Failed on import: unknown extension: " + extension);
		Core::LogMessage("File path: " + (std::string)file_path);
	}

	return nullptr;
}

std::vector<Core::Asset*> Core::AssetManager::GetAssets()
{
	return Get()->m_assets;
}

Core::AssetManager* Core::AssetManager::Get()
{
	if (!instance)
		instance = new Core::AssetManager();

	return instance;
}

void* Core::AssetManager::CreateModelData(std::string& _path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	Renderer::Model* model = new Renderer::Model();

	ProcessNode(scene->mRootNode, scene, model);

	model->SetShader(Renderer::default_shader);

	return model;
}

void* Core::AssetManager::CreateTextureData(std::string& _path)
{
	Renderer::Texture* _texture = new Renderer::Texture(_path.c_str());
	return _texture;
}

void* Core::AssetManager::CreateShaderData(std::string& _path)
{
	std::string matching_path = GetMatchingShader(_path);
	if (matching_path == "")
		return nullptr;

	if (IsVertShaderPath(_path))
		return new Renderer::Shader(_path.c_str(), matching_path.c_str());
	else
		return new Renderer::Shader(matching_path.c_str(), _path.c_str());

	return nullptr;
}

std::string Core::AssetManager::GetMatchingShader(std::string& path)
{
	const std::string frag_suffix = "_frag.glsl";
	const std::string vert_suffix = "_vert.glsl";

	if (IsVertShaderPath(path))
		return path.substr(0, path.size() - vert_suffix.size()) + frag_suffix;

	else
		return path.substr(0, path.size() - frag_suffix.size()) + vert_suffix;
	
	return "";
}

void Core::AssetManager::ProcessNode(void* _node, const void* _scene, Renderer::Model* _model)
{
	aiNode* node = (aiNode*)_node;
	const aiScene* scene = (aiScene*)_scene;

	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_model->m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, _model);
	}
}

Renderer::Mesh* Core::AssetManager::ProcessMesh(void* _mesh, const void* _scene)
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

	// TODO : make this rework again (texture assigned to material on import)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(material, (void*)aiTextureType_DIFFUSE, Renderer::TEXTURE_TYPE_DIFFUSE);
		LoadMaterialTextures(material, (void*)aiTextureType_SPECULAR, Renderer::TEXTURE_TYPE_NORMAL_MAP);
	}

	return new Renderer::Mesh(vertices, indices);
}

void Core::AssetManager::LoadMaterialTextures(void* _mat, void* _type, Renderer::TEXTURE_TYPE _type_name)
{
	aiMaterial* mat = (aiMaterial*)_mat;
	aiTextureType type = *(aiTextureType*)_type;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString _path;
		mat->GetTexture(type, i, &_path);

		ImportAsset(_path.C_Str());
	}
}

bool Core::AssetManager::IsVertShaderPath(std::string& path)
{
	const std::string vert_suffix = "_vert.glsl";

	return path.size() >= vert_suffix.size() &&	path.compare(path.size() - vert_suffix.size(), vert_suffix.size(), vert_suffix) == 0;
}
