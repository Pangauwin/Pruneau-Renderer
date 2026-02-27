#include "asset.h"

#pragma region MeshAsset

#include "renderer/mesh.h"

Core::MeshAsset::MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices, std::weak_ptr<ShaderAsset> _shader, const glm::mat4& _transform) 
	: Asset(std::move(_name), _id), m_mesh(std::make_unique<Renderer::Mesh>(vertices, indices, _shader.lock()->GetShader())), m_transform(_transform) {}
// TODO : replace _shader.lock()->GetShader() by something safer (weak_ptr can be empty)

void Core::MeshAsset::Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective)
{
	m_mesh->Draw(_view, _model, _perspective);
}
#pragma endregion

#pragma region TextureAsset

#include "renderer/texture.h"

Core::TextureAsset::TextureAsset(std::string _name, AssetID _id, void* _data, int _width, int _height) 
	: Asset(std::move(_name), _id), m_texture(std::make_unique<Renderer::Texture>(_data, _width, _height)) {}

#pragma endregion

#pragma region ShaderAsset

#include "renderer/shader.h"

Core::ShaderAsset::ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code) 
	: Asset(_name, _id), m_shader(std::make_shared<Renderer::Shader>(_vertex_shader_code, _fragment_shader_code)) {}

#pragma endregion

#pragma region ModelAsset

#include "renderer/model.h"

Core::ModelAsset::ModelAsset(std::string _name, AssetID _id, std::vector<std::shared_ptr<MeshAsset>> _meshes)
	: Asset(_name, _id), m_model(std::make_unique<Renderer::Model>(std::move(_meshes))) {}

#pragma endregion
