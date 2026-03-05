#pragma once

#include "asset.h"

#include <unordered_map>

struct ParsedModel;

namespace Core {

class Application;

// TODO : Import Settings / Preferences

class AssetManager
{
	friend class Application; // default/error materials and shader imports

public:
	template<typename T>
	static std::shared_ptr<T> GetAsset(AssetID _id);

	static AssetID ImportAsset(const std::string& path); // TODO : separate asset importer (create AssetImporter class)

	static void RemoveAsset(AssetID _id);

	static const std::unordered_map<AssetID, std::shared_ptr<Asset>>& GetAssets() { return m_assets; }

	static std::shared_ptr<ShaderAsset> error_shader;
	static std::shared_ptr<ShaderAsset> default_shader;

	static std::shared_ptr<MaterialAsset> default_material;
	static std::shared_ptr<MaterialAsset> error_material;

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

	static AssetID s_nextID;
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