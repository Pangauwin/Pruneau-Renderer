#pragma once

#include "asset.h"
#include <renderer/model.h>

namespace Core {

// TODO : Import Settings / Preferences

class AssetManager
{
public:
	AssetManager();

	static Asset* ImportAsset(const char* file_path);
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

	std::vector<Asset*> m_assets;
};

}