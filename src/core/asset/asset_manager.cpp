#include "asset_manager.h"

#include <string>

#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/application.h"

#include "renderer/texture.h"

namespace Core {
	std::unordered_map<AssetID, std::shared_ptr<Asset>> AssetManager::m_assets;
	AssetID AssetManager::s_nextID = 1;
}

/*
TODO: Create import failed states and warnings on import
*/

Core::AssetID Core::AssetManager::ImportAsset(const std::string& path)
{
	AssetID id = 0;

	if (EndsWith(path, ".obj") || EndsWith(path, ".fbx"))
	{
		id = ImportModel(std::move(path));
	}

	else if (EndsWith(path, ".png") || EndsWith(path, ".jpg") || EndsWith(path, ".bmp"))
	{
		id = ImportTexture(std::move(path));
	}

	else if (EndsWith(path, ".glsl"))
	{
		id = ImportShader(path);
	}

	else
	{
		Core::LogMessage("Asset format not handled:" + path);
		return 0;
	}

	if (!id)
		Core::LogMessage("Could not import asset:" + path);

	return id;
}

void Core::AssetManager::RemoveAsset(AssetID _id)
{
	Core::AssetManager::m_assets.erase(_id);
}

Core::AssetID Core::AssetManager::ImportModel(const std::string& _path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	std::vector<std::weak_ptr<Core::Asset>> _meshes;

	ProcessNode(scene->mRootNode, scene, _meshes);

	s_nextID++;

	Core::AssetManager::m_assets[s_nextID] = std::make_shared<ModelAsset>("Model_" + std::to_string(s_nextID), s_nextID, _meshes);

	return s_nextID;

	//TODO #1 : apply the right shader to the meshes
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

Core::AssetID Core::AssetManager::ImportTexture(const std::string& path)
{
	int width, height, channel_nb;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel_nb, 4);

	if (!data)
	{
		stbi_image_free(data);
		return 0;
	}

	s_nextID++;
	Core::AssetManager::m_assets[s_nextID] = std::make_shared<TextureAsset>("Texture_" + s_nextID, s_nextID, data, width, height);

	stbi_image_free(data);

	return s_nextID;
}

Core::AssetID Core::AssetManager::ImportShader(const std::string& path)
{
	std::string vertex_shader_file_path = path;
	std::string fragment_shader_file_path = path;

	if (EndsWith(path, "_vert.glsl"))
	{
		size_t pos = fragment_shader_file_path.rfind("_vert.glsl");
		fragment_shader_file_path.replace(pos, 10, "_frag.glsl");
	}

	else if (EndsWith(path, "_frag.glsl"))
	{
		size_t pos = vertex_shader_file_path.rfind("_frag.glsl");
		vertex_shader_file_path.replace(pos, 10, "_vert.glsl");
	}

	else
	{
		Core::LogMessage("Could not import Shader, make sure the filename ends with _vert.glsl or _frag.glsl (underscore included)");
		return 0;
	}

	std::string vertex_shader, fragment_shader;

	std::ifstream vertex_file(vertex_shader_file_path, std::ios::in | std::ios::binary);
	std::ostringstream vertex_shader_contents;

	if (!vertex_file)
	{
		Core::LogMessage("Failed to open the vertex shader file");
		return 0;
	}

	vertex_shader_contents << vertex_file.rdbuf();
	vertex_shader = vertex_shader_contents.str();

	std::ifstream fragment_file(fragment_shader_file_path, std::ios::in | std::ios::binary);
	std::ostringstream fragment_shader_contents;

	if (!fragment_file)
	{
		Core::LogMessage("Failed to open the fragment shader file");
		return 0;
	}

	fragment_shader_contents << fragment_file.rdbuf();
	fragment_shader = fragment_shader_contents.str();

	s_nextID++;
	Core::AssetManager::m_assets[s_nextID] = std::make_shared<ShaderAsset>("Shader_" + std::to_string(s_nextID), s_nextID, vertex_shader.c_str(), fragment_shader.c_str());

	return s_nextID;
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

	s_nextID++;
	Core::AssetManager::m_assets[s_nextID] = std::make_shared<MeshAsset>("Mesh_" + std::to_string(s_nextID), s_nextID, vertices, indices); // TODO : fix : add shader inside constructor

	return Core::AssetManager::m_assets[s_nextID];
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