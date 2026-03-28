#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <variant>

#include <glm/fwd.hpp>

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
using FolderID = uint32_t;

class Asset
{
public:
	Asset(std::string _name, AssetID _id) : name(std::move(_name)), m_id(_id) {}

	// TODO : Make so that the assets register themselves inside

	virtual ~Asset() = default;

	const std::string& GetName() const { return name; }
	AssetID GetID() const { return m_id; }

	virtual void OnGUIRender() {}
	virtual void OnContextMenuRender() {}

public:
	std::string name;
	FolderID folder = 0;

private:
	AssetID m_id;
};

// TODO : Implement OnGuiRender on all Assets

class ShaderAsset : public Asset
{
public:
	ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code);

	std::shared_ptr<Renderer::Shader> GetShader() { return m_shader; }

	void OnGUIRender();
	void OnContextMenuRender();

private:
	std::shared_ptr<Renderer::Shader> m_shader;
};

class TextureAsset : public Asset
{
public:
	TextureAsset(std::string _name, AssetID _id, void* _data, int _width, int _height);
	~TextureAsset();

	Renderer::Texture* GetTexture() { return m_texture.get(); }
	void Bind(int _slot);

	void OnGUIRender();

private:
	std::unique_ptr<Renderer::Texture> m_texture;
};

class MaterialAsset : public Asset
{
public:
	using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

public:
	MaterialAsset(std::string _name, AssetID _id, std::shared_ptr<ShaderAsset> _shader);

	void SetTexture(const std::string& uniform_name, std::shared_ptr<TextureAsset> _texture);
	void SetUniform(const std::string& uniform_name, const UniformValue& _value);

	std::shared_ptr<ShaderAsset> GetShaderAsset() const { return m_shader; }

	void Bind();

	void OnGUIRender();

private:
	void UploadUniform(const std::string& name, const UniformValue& _value);

private:
	std::shared_ptr<ShaderAsset> m_shader;

	std::unordered_map<std::string, std::shared_ptr<TextureAsset>> m_textures;
	std::unordered_map<std::string, UniformValue> m_uniforms;
};

class MeshAsset : public Asset
{
public:
	MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices, std::shared_ptr<MaterialAsset> _shader);

	void Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective);

	void OnGUIRender();

	const Renderer::Mesh* GetMesh() const { return m_mesh.get(); }

private:
	std::unique_ptr<Renderer::Mesh> m_mesh;
};

class ModelAsset : public Asset
{
public:
	ModelAsset(std::string _name, AssetID _id, std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes);
	~ModelAsset();

	const Renderer::Model* GetModel() const { return m_model.get(); }

	void OnGUIRender();

private:
	std::unique_ptr<Renderer::Model> m_model;
};

// TODO : Implement Audio Assets
}