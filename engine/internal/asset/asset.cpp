#include "asset/asset.h"
#include "asset/asset_manager.h"

#include <cstdint>
#include <random>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "asset/asset_manager.h"

#include <glad/glad.h>

Core::AssetGUID Core::GenerateGUID()
{
	static thread_local std::mt19937_64 rng(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> dist;
	return { dist(rng), dist(rng)};
}

#include "core/application.h"

#pragma region MeshAsset

#include "renderer/mesh.h"

Core::MeshAsset::MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices) 
	: Asset(_name, _id) 
{
	m_mesh = std::make_unique<Renderer::Mesh>(vertices, indices);
}

#pragma endregion

#pragma region TextureAsset

#include "renderer/texture.h"

Core::TextureAsset::TextureAsset(std::string _name, AssetID _id, void* _data, int _width, int _height) 
	: Asset(_name, _id), m_texture(std::make_unique<Renderer::Texture>(_data, _width, _height)) {}

void Core::TextureAsset::Bind(int _slot)
{
	m_texture.get()->Bind(_slot);
}

namespace Core {
	TextureAsset::~TextureAsset() = default;
}

#pragma endregion

#pragma region ShaderAsset

#include "renderer/shader.h"

Core::ShaderAsset::ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code) 
	: Asset(_name, _id), m_shader(std::make_shared<Renderer::Shader>(_vertex_shader_code, _fragment_shader_code)) {}

#pragma endregion

#pragma region ModelAsset

Core::ModelAsset::ModelAsset(std::string _name, AssetID _id, std::vector<ModelReadyMeshData>& _meshes)
	: Asset(_name, _id), m_meshes(std::move(_meshes)) 
	{} // TODO: Check if here the mat4 is still there


void Core::ModelAsset::ModelAsset::Draw(const glm::mat4& _projection, const glm::mat4& _view, const glm::mat4& _model)
{
	for(auto& mesh: m_meshes)
	{
		std::shared_ptr<Core::MaterialAsset> _mat = Core::AssetManager::GetAsset<Core::MaterialAsset>(mesh.materialID);
		std::shared_ptr<Core::MeshAsset> _mesh = Core::AssetManager::GetAsset<Core::MeshAsset>(mesh.meshID);
		glm::mat4 transform = mesh.mesh_transform;
		
		if (_mat)
		{
			_mat->Bind();
			
			_mat->GetShaderAsset()->GetShader()->SetMat4("view", _view);
			_mat->GetShaderAsset()->GetShader()->SetMat4("model", _model * transform);
			_mat->GetShaderAsset()->GetShader()->SetMat4("perspective", _projection);
		}

		else
		{
			Core::AssetManager::error_material->Bind();

			Core::AssetManager::error_material->GetShaderAsset()->GetShader()->SetMat4("view", _view);
			Core::AssetManager::error_material->GetShaderAsset()->GetShader()->SetMat4("model", _model * transform);
			Core::AssetManager::error_material->GetShaderAsset()->GetShader()->SetMat4("perspective", _projection);
		}

		glBindVertexArray(_mesh->GetMesh()->VAO);
		glDrawElements(GL_TRIANGLES, _mesh->GetMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

}

void Core::MeshAsset::DrawWireframe(const glm::mat4& _projection, const glm::mat4& _view, const glm::mat4& _mesh)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Core::AssetManager::collider_material->Bind();

	Core::AssetManager::collider_material->GetShaderAsset()->GetShader()->SetMat4("view", _view);
	Core::AssetManager::collider_material->GetShaderAsset()->GetShader()->SetMat4("model", _mesh);
	Core::AssetManager::collider_material->GetShaderAsset()->GetShader()->SetMat4("perspective", _projection);

	glBindVertexArray(GetMesh()->VAO);
	glDrawElements(GL_TRIANGLES, GetMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

namespace Core {
	ModelAsset::~ModelAsset() = default;
}

#pragma endregion

#pragma region MaterialAsset

Core::MaterialAsset::MaterialAsset(std::string _name, AssetID _id, std::shared_ptr<ShaderAsset> _shader) : Asset(_name, _id), m_shader(std::move(_shader)) {}

void Core::MaterialAsset::SetTexture(const std::string& uniform_name, AssetID _texture)
{
	if(std::shared_ptr<TextureAsset> texture_asset = AssetManager::GetAsset<TextureAsset>(_texture))
		m_textures[uniform_name] = std::move(texture_asset);
	else
		Core::LogMessageError("Error while passing texture parameter: texture asset not valid");
}

void Core::MaterialAsset::SetUniform(const std::string& uniform_name, const UniformValue& _value)
{
	m_uniforms[uniform_name] = _value;
}

void Core::MaterialAsset::Bind()
{
	if (!m_shader)
		return;

	auto _shader = m_shader->GetShader();
	_shader->Bind();

	int texture_slot = 0;

	for (auto& [uniform_name, texture_asset] : m_textures)
	{
		if (!texture_asset)
			continue;

		Renderer::Texture* _texture = texture_asset->GetTexture();

		if (!_texture)
			continue;

		_texture->Bind(texture_slot);

		_shader->SetInt(uniform_name, texture_slot);

		texture_slot++;
	}

	for (auto& [uniform_name, value] : m_uniforms)
	{
		UploadUniform(uniform_name, value);
	}

}

void Core::MaterialAsset::UploadUniform(const std::string& name, const UniformValue& _value)
{
	std::shared_ptr<Renderer::Shader> _shader = m_shader->GetShader();

	std::visit([&](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, int>)
				_shader->SetInt(name, arg);

			else if constexpr (std::is_same_v<T, float>)
				_shader->SetFloat(name, arg);

			else if constexpr (std::is_same_v<T, glm::vec2>)
				_shader->SetVec2(name, arg);

			else if constexpr (std::is_same_v<T, glm::vec3>)
				_shader->SetVec3(name, arg);

			else if constexpr (std::is_same_v<T, glm::vec4>)
				_shader->SetVec4(name, arg);

			else if constexpr (std::is_same_v<T, glm::mat4>)
				_shader->SetMat4(name, arg);
		}, _value);
}


#pragma endregion
