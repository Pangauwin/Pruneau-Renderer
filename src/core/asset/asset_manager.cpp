#include "asset_manager.h"

#include <string>

#include "renderer/model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/application.h"

Core::AssetManager* instance = nullptr;

std::vector<Renderer::Texture*> loaded_textures;

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

		// TODO : Create failed states
		Get()->m_assets.push_back(_asset);

		return _asset;
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
	std::vector<Renderer::Texture*> textures;

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

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiTextureType* type = new aiTextureType();
		(*type) = aiTextureType_DIFFUSE;
		std::vector<Renderer::Texture*> diffuseMaps = LoadMaterialTextures(material, type, Renderer::TEXTURE_TYPE_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		(*type) = aiTextureType_SPECULAR;
		std::vector<Renderer::Texture*> specularMaps = LoadMaterialTextures(material, type, Renderer::TEXTURE_TYPE_NORMAL_MAP);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return new Renderer::Mesh(vertices, indices, textures);
}

std::vector<Renderer::Texture*> Core::AssetManager::LoadMaterialTextures(void* _mat, void* _type, Renderer::TEXTURE_TYPE _type_name)
{
	std::vector<Renderer::Texture*> textures;
	aiMaterial* mat = (aiMaterial*)_mat;
	aiTextureType type = *(aiTextureType*)_type;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(loaded_textures[j]->path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Renderer::Texture* texture = new Renderer::Texture(str.C_Str(), _type_name);
			textures.push_back(texture);
			loaded_textures.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}
