#pragma once

#include "asset.h"

#include <memory>
#include <unordered_map>

struct ParsedModel;

namespace Core {

class Application;

using FolderID = uint32_t;

struct AssetFolder {
	FolderID id;

	std::string name;

	FolderID parent = 0;

	std::vector<std::shared_ptr<Core::Asset>> assets;
	std::vector<AssetFolder*> children;
};

class AssetManager
{
	friend class Application; // default/error materials and shader imports
	friend class Asset;

public:

	static void Init();

#pragma region Folders
	static FolderID CreateFolder(const std::string& name, FolderID parent = 0);
	static AssetFolder& GetFolder(FolderID _id);
	static void AssignAssetToFolder(AssetID _asset_id, FolderID _folder_id);

	static void InitializeRootFolder()
	{
		if (m_folders.empty())
			CreateFolder("Root", 0);
	}

#pragma endregion

#pragma region Assets
	template<typename T>
	static std::shared_ptr<T> GetAsset(AssetID _id);

	static AssetID ImportAsset(const std::string& path, FolderID _folder = 0); // TODO : separate asset importer (create AssetImporter class)

	static void RemoveAsset(AssetID _id);

	static const std::unordered_map<AssetID, std::shared_ptr<Asset>>& GetAssets() { return m_assets; }

	static std::shared_ptr<ShaderAsset> error_shader;
	static std::shared_ptr<ShaderAsset> default_shader;

	static std::shared_ptr<MaterialAsset> default_material;
	static std::shared_ptr<MaterialAsset> error_material;
#pragma endregion

#pragma region MaterialCreation
	static AssetID CreateMaterial(std::shared_ptr<ShaderAsset> _shader);
#pragma endregion


private:
	AssetManager() = default;

	static void SetDefaultShader(const char* _path);
	static void SetErrorShader(const char* _path);

#pragma region ModelLoading
	static AssetID ImportModel(const std::string& _path);

	static Core::AssetID BuildModelAsset(const ParsedModel& parsed);
#pragma endregion

#pragma region TextureLoading
	static AssetID ImportTexture(const std::string& path);
#pragma endregion
	
#pragma region ShaderLoading
	static AssetID ImportShader(const std::string& path);
#pragma endregion

	static std::unordered_map<AssetID, std::shared_ptr<Asset>> m_assets;
	static std::unordered_map<AssetID, std::unique_ptr<AssetFolder>> m_folders;

	static FolderID s_next_folder_id;
	static AssetID s_next_asset_id;
};

}

template<typename T>
std::shared_ptr<T> Core::AssetManager::GetAsset(AssetID id)
{
	auto it = m_assets.find(id);

	if (it == m_assets.end())
		return nullptr;

	return std::dynamic_pointer_cast<T>(it->second);
}