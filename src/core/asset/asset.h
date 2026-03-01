#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

#include <glm/glm.hpp>

namespace Renderer
{
	class Mesh;
	class Texture;
	class Shader;
	class Model;
	struct Vertex;
}

namespace Core {

/*
* NOTE #1 : Each asset object is created with its own data (independant from the data source)
* NOTE #2 : Each asset is identified with the assetID;
* NOTE #3 : When the project is closed, the editor will check if any of the assets have changed. If so, it will edit the data inside the asset file (in coming)
* 
* TODO : Implement asset storage on the disk
* TODO : Implement asset importer from the asset files (those who belongs to the Pruneau Engine)
*/

using AssetID = uint32_t;

class Asset
{
public:
	Asset(std::string _name, AssetID _id) : name(std::move(_name)), m_id(_id) {}

	virtual ~Asset() = default;

	const std::string& GetName() const { return name; }
	AssetID GetID() const { return m_id; }

	virtual void OnGUIRender() {}

private:
	std::string name;
	AssetID m_id;
};

class ShaderAsset : public Asset
{
public:
	ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code);

	std::shared_ptr<Renderer::Shader> GetShader() { return m_shader; }

private:
	std::shared_ptr<Renderer::Shader> m_shader;
};

class MeshAsset : public Asset
{
public:
	MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices, std::weak_ptr<ShaderAsset> _shader);

	void Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective);

private:
	std::unique_ptr<Renderer::Mesh> m_mesh;
};

class TextureAsset : public Asset
{
public:
	TextureAsset(std::string _name, AssetID _id, void* _data, int _width, int _height); // TODO : add texture settings

private:
	std::unique_ptr<Renderer::Texture> m_texture;
};

class ModelAsset : public Asset
{
public:
	ModelAsset(std::string _name, AssetID _id, std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes);

	const Renderer::Model* GetModel() const { return m_model.get(); }

private:
	std::unique_ptr<Renderer::Model> m_model;
};

// TODO : Implement MaterialAsset (shader + textures + other shader parameters) => On use : Material does everything
// TODO : Implement Audio Assets


// TODO : Implement Instances (MeshInstance)
}