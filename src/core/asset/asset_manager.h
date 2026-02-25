#pragma once

#include "asset.h"

#include <unordered_map>

namespace Core {

// TODO : Import Settings / Preferences

class AssetManager
{
public:
	template<typename T>
	static std::shared_ptr<T> GetAsset(AssetID _id);

	static AssetID ImportAsset(const std::string& path); // TODO : separate asset importer (create AssetImporter class)

	static void RemoveAsset(AssetID _id);

private:
	AssetManager() = default;

#pragma region ModelLoading
	static void ImportModel(std::string _path);

	static std::weak_ptr<Core::Asset> Core::AssetManager::ProcessMesh(void* _mesh, const void* _scene);

	static void ProcessNode(void* _node, const void* _scene, std::vector<std::weak_ptr<Core::Asset>>& _meshes);
#pragma endregion

	static std::unordered_map<AssetID, std::shared_ptr<Asset>> m_assets;

	/*static Asset* ImportAsset(const char* file_path);
	static std::vector<Asset*> GetAssets();

	static AssetManager* Get();

private:
	void* CreateModelData(std::string& _path);
	void* CreateTextureData(std::string& _path);
	void* CreateShaderData(std::string& _path);

#pragma region ModelImport
	void ProcessNode(void* _node, const void* _scene, Renderer::Model* _model);
	Renderer::Mesh* ProcessMesh(void* _mesh, const void* _scene);

	void LoadMaterialTextures(void* _mat, void* _type, Renderer::TEXTURE_TYPE _type_name);
#pragma endregion

#pragma region ShaderImport
	std::string Core::AssetManager::GetMatchingShader(std::string& path);
	bool IsVertShaderPath(std::string& path);
#pragma endregion

	std::vector<Asset*> m_assets;*/
	// TODO : Clean this old code
};

}