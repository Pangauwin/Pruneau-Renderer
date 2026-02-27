#pragma once

#include "asset.h"

#include <unordered_map>

struct ParsedModel;

namespace Core {

// TODO : Import Settings / Preferences

class AssetManager
{
public:
	template<typename T>
	static std::shared_ptr<T> GetAsset(AssetID _id);

	static AssetID ImportAsset(const std::string& path); // TODO : separate asset importer (create AssetImporter class)

	static void RemoveAsset(AssetID _id);

	static const std::unordered_map<AssetID, std::shared_ptr<Asset>>& GetAssets() { return m_assets; }

private:
	AssetManager() = default;

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

	static std::shared_ptr<ShaderAsset> default_shader; // TODO : implement default_shader registering inside the assets pool
	// TODO (better) : Add a default material instead of a default shader that holds everything (requires first material implementation)
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