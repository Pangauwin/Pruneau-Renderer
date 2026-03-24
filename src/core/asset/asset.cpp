#include "asset.h"
#include "asset_manager.h"

#include <imgui.h>

#include <memory>
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
	m_mesh->Draw(_view, _model, _perspective);
}
void Core::MeshAsset::OnGUIRender()
{
	ImGui::Text("%s", ("Asset ID: " + std::to_string(GetID())).c_str());
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
	ImGui::Text("%s",("Asset ID: " + std::to_string(GetID())).c_str());
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
	ImGui::Text("%s",("Asset ID: " + std::to_string(GetID())).c_str());
}

void Core::ShaderAsset::OnContextMenuRender()
{
	if(ImGui::MenuItem("Create Material from Shader"))
	{
		AssetManager::CreateMaterial(AssetManager::GetAsset<ShaderAsset>(this->GetID()));
	}
}

#pragma endregion

#pragma region ModelAsset

#include "renderer/model.h"

Core::ModelAsset::ModelAsset(std::string _name, AssetID _id, std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes)
	: Asset(_name, _id), m_model(std::make_unique<Renderer::Model>(std::move(_meshes))) {}

void Core::ModelAsset::OnGUIRender()
{
	ImGui::InputText(("Asset Name##" + std::to_string(GetID())).c_str(), &name);

	ImGui::Text("%s",("Asset ID: " + std::to_string(GetID())).c_str());

	ImGui::Text("Internal meshes ids: ");
	for (auto& _mesh : m_model->GetMeshes())
	{
		std::shared_ptr<MeshAsset> _asset = std::get<std::shared_ptr<MeshAsset>>(_mesh);
		ImGui::BulletText("%s", _asset->GetName().c_str());

		int id = _asset->GetMesh()->m_material->GetID();

		if(ImGui::DragInt("MaterialID", &id))
		{
			if(std::shared_ptr<Core::MaterialAsset> _mat = AssetManager::GetAsset<MaterialAsset>(id))
			{
				_asset->GetMesh()->m_material = _mat;
			}
		}
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
	ImGui::Text("Asset ID: %s" , std::to_string(GetID()).c_str());

	if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (auto& [uniform_name, tex] : m_textures)
        {
            if (!tex) continue;

            ImGui::Text("%s:", uniform_name.c_str());
            ImGui::SameLine();

            Renderer::Texture* texture = tex->GetTexture();
            if (texture)
            {
                ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2(64,64));
            }
        }
    }

	if (ImGui::CollapsingHeader("Uniforms", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto it = m_uniforms.begin(); it != m_uniforms.end(); )
		{
			auto& [name, value] = *it;

			ImGui::PushID(name.c_str());

			ImGui::Text("%s", name.c_str());
			ImGui::SameLine();

			bool remove = false;

			std::visit([&](auto& arg)
			{
				using T = std::decay_t<decltype(arg)>;

				if constexpr (std::is_same_v<T, int>)
					ImGui::DragInt("##v", &arg);

				else if constexpr (std::is_same_v<T, float>)
					ImGui::DragFloat("##v", &arg, 0.1f);

				else if constexpr (std::is_same_v<T, glm::vec2>)
					ImGui::DragFloat2("##v", &arg.x, 0.1f);

				else if constexpr (std::is_same_v<T, glm::vec3>)
					ImGui::ColorEdit3("##v", &arg.x);

				else if constexpr (std::is_same_v<T, glm::vec4>)
					ImGui::ColorEdit4("##v", &arg.x);

				else if constexpr (std::is_same_v<T, glm::mat4>)
				{
					ImGui::Text("mat4");
				}

			}, value);

			ImGui::SameLine();

			if (ImGui::Button("X"))
				remove = true;

			ImGui::PopID();

			if (remove)
				it = m_uniforms.erase(it);
			else
				++it;
		}

		if (ImGui::Button("Add Uniform"))
			ImGui::OpenPopup("AddUniformPopup");

		if (ImGui::BeginPopup("AddUniformPopup"))
		{
			static char name[64] = "";
			static int type = 0;

			const char* types[] = { "int", "float", "vec2", "vec3", "vec4" };

			ImGui::InputText("Name", name, 64);
			ImGui::Combo("Type", &type, types, IM_ARRAYSIZE(types));

			if (ImGui::Button("Create"))
			{
				if (strlen(name) > 0 && m_uniforms.find(name) == m_uniforms.end())
				{
					switch (type)
					{
						case 0: m_uniforms[name] = 0; break;
						case 1: m_uniforms[name] = 0.0f; break;
						case 2: m_uniforms[name] = glm::vec2(0.0f); break;
						case 3: m_uniforms[name] = glm::vec3(1.0f); break;
						case 4: m_uniforms[name] = glm::vec4(1.0f); break;
					}

					name[0] = '\0';
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
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
