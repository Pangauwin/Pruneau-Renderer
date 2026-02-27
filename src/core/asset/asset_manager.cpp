#include "asset_manager.h"

#include <string>

#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/application.h"


namespace Core {
	std::unordered_map<AssetID, std::shared_ptr<Asset>> AssetManager::m_assets;
	AssetID AssetManager::s_nextID = 1;
}

std::weak_ptr<Core::ShaderAsset> Core::AssetManager::default_shader;
//TODO : initialize this pointer : 
/*
AssetID shaderID = ImportShader("default_vert.glsl");
default_shader = GetAsset<ShaderAsset>(shaderID);
*/

struct ParsedMesh {
	std::vector<Renderer::Vertex> vertices;
	std::vector<uint32_t> indices;
	int materialIndex;
	glm::mat4 transform;
};

struct ParsedMaterial
{
	std::vector<std::string> diffuse_textures;
};

struct ParsedModel
{
	std::vector<ParsedMesh> meshes;
	std::vector<ParsedMaterial> materials;
};

Core::AssetID Core::AssetManager::ImportAsset(const std::string& path)
{
	AssetID id = 0;

	if (EndsWith(path, ".obj") || EndsWith(path, ".gltf") || EndsWith(path, ".glb") || EndsWith(path, ".fbx"))
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

	unsigned int import_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality;

	if (EndsWith(_path, ".obj"))
		import_flags |= aiProcess_FlipUVs;

	const aiScene* scene = importer.ReadFile(_path, import_flags);

	if (!scene || !scene->mRootNode)
	{
		Core::LogMessage("Assimp error: " + std::string(importer.GetErrorString()));
		return 0;
	}

	ParsedModel parsed_model;

	ParseMaterials(scene, parsed_model);
	ParseNode(scene->mRootNode, scene, glm::mat4(1.0f), parsed_model);

	return BuildModelAsset(parsed_model);

	//TODO #1 : apply the right shader to the meshes
	//TODO #2 : apply the right transform of the meshes (in the node) inside the model
}

static void ParseMaterials(const aiScene* _scene, ParsedModel& _model)
{
	for (unsigned int i = 0; i < _scene->mNumMaterials; i++)
	{
		ParsedMaterial mat;

		aiMaterial* material = _scene->mMaterials[i];

		aiString path;

		for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); j++)
		{
			material->GetTexture(aiTextureType_DIFFUSE, j, &path);
			mat.diffuse_textures.push_back(path.C_Str());
		}

		_model.materials.push_back(mat);
	}
}

static void ParseNode(aiNode* node, const aiScene* scene, const glm::mat4& _parent_transform, ParsedModel& _model)
{
	glm::mat4 local_transform = ConvertMatrix(node->mTransformation);
	glm::mat4 global_transform = _parent_transform * local_transform;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		ParsedMesh parsed_mesh = ParseMesh(mesh);
		parsed_mesh.transform = global_transform;

		_model.meshes.push_back(parsed_mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ParseNode(node->mChildren[i], scene, global_transform, _model);
	}
}

static ParsedMesh ParseMesh(aiMesh* _mesh)
{
	ParsedMesh parsed_mesh;

	for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
	{
		Renderer::Vertex vertex{};

		vertex.position = {
			_mesh->mVertices[i].x,
			_mesh->mVertices[i].y,
			_mesh->mVertices[i].z
		};

		if (_mesh->HasNormals())
		{
			vertex.normal = {
				_mesh->mNormals[i].x,
				_mesh->mNormals[i].y,
				_mesh->mNormals[i].z
			};
		}

		if (_mesh->HasTextureCoords(0))
		{
			vertex.uv_coord = {
				_mesh->mTextureCoords[0][i].x,
				_mesh->mTextureCoords[0][i].y
			};
		}

		parsed_mesh.vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace& face = _mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			parsed_mesh.indices.push_back(face.mIndices[j]);
		}
	}

	parsed_mesh.materialIndex = _mesh->mMaterialIndex;

	return parsed_mesh;
}

Core::AssetID Core::AssetManager::BuildModelAsset(const ParsedModel& parsed)
{
	std::vector<std::weak_ptr<Core::MeshAsset>> mesh_assets;

	for (const ParsedMesh& mesh : parsed.meshes)
	{
		s_nextID++;

		auto mesh_asset = std::make_shared<MeshAsset>(
			"Mesh_" + std::to_string(s_nextID),
			s_nextID,
			mesh.vertices,
			mesh.indices,
			default_shader,
			mesh.transform
		);

		m_assets[s_nextID] = mesh_asset;

		mesh_assets.push_back(mesh_asset);
	}

	s_nextID++;

	auto model = std::make_shared<ModelAsset>(
		"Model_" + std::to_string(s_nextID),
		s_nextID,
		mesh_assets
	);

	m_assets[s_nextID] = model;

	return s_nextID;
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

static bool EndsWith(const std::string& value, const std::string& ending)
{
	if (ending.size() > value.size())
		return false;

	return std::equal(
		ending.rbegin(), ending.rend(),
		value.rbegin()
	);
}

static glm::mat4 ConvertMatrix(const aiMatrix4x4& m)
{
	return glm::mat4(
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4
	);
}