#include "asset.h"

#include <imgui.h>

#include <misc/cpp/imgui_stdlib.cpp>

#include "asset_manager.h"

#include "core/application.h"

#pragma region MeshAsset

#include "renderer/mesh.h"

Core::MeshAsset::MeshAsset(std::string _name, AssetID _id, const std::vector<Renderer::Vertex>& vertices, const std::vector<unsigned int>& indices, std::shared_ptr<MaterialAsset> _material) 
	: Asset(std::move(_name), _id) 
{
	if (_material)
	{
		m_mesh = std::make_unique<Renderer::Mesh>(vertices, indices, _material);
	}
	else
	{
		m_mesh = std::make_unique<Renderer::Mesh>(vertices, indices, Core::AssetManager::error_material);
		Core::LogMessage("The material assigned to the model is not valid, error material assigned instead. ModelID:" + std::to_string(_id));
	}
}

void Core::MeshAsset::Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective)
{
	Core::LogMessageDebug("mesh transform: " + std::to_string(_model[3].x));
	m_mesh->Draw(_view, _model, _perspective);
}
void Core::MeshAsset::OnGUIRender()
{
	ImGui::Text(("Asset ID: " + std::to_string(GetID())).c_str());
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

void Core::TextureAsset::OnGUIRender()
{
	ImGui::Text(("Asset ID: " + std::to_string(GetID())).c_str());
}

namespace Core {
	TextureAsset::~TextureAsset() = default;
}

#pragma endregion

#pragma region ShaderAsset

#include "renderer/shader.h"

Core::ShaderAsset::ShaderAsset(std::string _name, AssetID _id, const char* _vertex_shader_code, const char* _fragment_shader_code) 
	: Asset(_name, _id), m_shader(std::make_shared<Renderer::Shader>(_vertex_shader_code, _fragment_shader_code)) {}

void Core::ShaderAsset::OnGUIRender()
{
	ImGui::Text(("Asset ID: " + std::to_string(GetID())).c_str());
}

#pragma endregion

#pragma region ModelAsset

#include "renderer/model.h"

Core::ModelAsset::ModelAsset(std::string _name, AssetID _id, std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes)
	: Asset(_name, _id), m_model(std::make_unique<Renderer::Model>(std::move(_meshes))) {}

void Core::ModelAsset::OnGUIRender()
{
	if (ImGui::InputText(("Asset Name##" + std::to_string(GetID())).c_str(), &name));

	ImGui::Text(("Asset ID: " + std::to_string(GetID())).c_str());

	ImGui::Text("Internal meshes ids: ");
	for (auto& _mesh : m_model->GetMeshes())
	{
		std::shared_ptr<MeshAsset> _asset = std::get<std::shared_ptr<MeshAsset>>(_mesh);
		ImGui::BulletText(_asset->GetName().c_str());
	}
}

namespace Core {
	ModelAsset::~ModelAsset() = default;
}

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

void Core::MaterialAsset::OnGUIRender()
{
	ImGui::Text(("Asset ID: " + std::to_string(GetID())).c_str());
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
