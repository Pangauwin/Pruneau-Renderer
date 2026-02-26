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
	static AssetID ImportModel(const std::string& _path);

	static std::weak_ptr<Asset> ProcessMesh(void* _mesh, const void* _scene);

	static void ProcessNode(void* _node, const void* _scene, std::vector<std::weak_ptr<Core::Asset>>& _meshes);
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