#include "asset.h"
#include "asset_manager.h"

#include "core/application.h"

#pragma region MeshAsset

#include "renderer/mesh.h"

Core::MeshAsset::MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices, std::weak_ptr<ShaderAsset> _shader) 
	: Asset(std::move(_name), _id) 
{
	if (_shader.lock())
	{
		m_mesh = std::make_unique<Renderer::Mesh>(vertices, indices, _shader.lock()->GetShader());
	}
	else
	{
		m_mesh = std::make_unique<Renderer::Mesh>(vertices, indices, Core::AssetManager::error_shader->GetShader());
		Core::LogMessage("The shader assigned to the model, error shader assigned instead. ModelID:" + std::to_string(_id));
	}
}

void Core::MeshAsset::Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective)
{
	m_mesh->Draw(_view, _model, _perspective);
}
#pragma endregion

#pragma region TextureAsset

#include "renderer/texture.h"

Core::TextureAsset::TextureAsset(std::string _name, AssetID _id, void* _data, int _width, int _height) 
	: Asset(std::move(_name), _id), m_texture(std::make_unique<Renderer::Texture>(_data, _width, _height)) {}

void Core::TextureAsset::Bind(int _slot)
{
	m_texture.get()->Bind(_slot);
}

#pragma endregion

#pragma region ShaderAsset

#include "renderer/shader.h"

Core::ShaderAsset::ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code) 
	: Asset(_name, _id), m_shader(std::make_shared<Renderer::Shader>(_vertex_shader_code, _fragment_shader_code)) {}

#pragma endregion

#pragma region ModelAsset

#include "renderer/model.h"

Core::ModelAsset::ModelAsset(std::string _name, AssetID _id, std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes)
	: Asset(_name, _id), m_model(std::make_unique<Renderer::Model>(std::move(_meshes))) {}

#pragma endregion

#pragma region MaterialAsset

Core::MaterialAsset::MaterialAsset(std::string _name, AssetID _id, std::shared_ptr<ShaderAsset> _shader) : Asset(_name, _id), m_shader(std::move(_shader)) {}

void Core::MaterialAsset::SetTexture(const std::string& uniform_name, std::shared_ptr<TextureAsset> _texture)
{
	m_textures[uniform_name] = std::move(_texture);
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
