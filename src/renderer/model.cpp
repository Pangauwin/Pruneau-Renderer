#include "model.h"

Renderer::Model::Model(std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes) : m_meshes(std::move(_meshes)) {}

void Renderer::Model::Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective)
{
    /*unsigned int diffuse_nb = 0;
    unsigned int normal_nb = 0;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        /*
        For this part of code, we are assuming that the textures in the shader file are written like so :
        uniform sampler2D texture_diffuse_1;
        uniform sampler2D texture_diffuse_2;
        uniform sampler2D texture_diffuse_3;
        ...
        uniform sampler2D texture_normal_1;
        uniform sampler2D texture_normal_2;
        uniform sampler2D texture_normal_3;
        ...
        *//*

        std::string name;
        switch (m_textures[i]->type)
        {
        case TEXTURE_TYPE_DIFFUSE:
            diffuse_nb++;
            name = "texture_diffuse_" + std::to_string(diffuse_nb);
            break;
        case TEXTURE_TYPE_NORMAL_MAP:
            normal_nb++;
            name = "texture_normal_" + std::to_string(normal_nb);
            break;
        default:
            break;
        }

        shader->SetInt(name.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetID());
    }
    // TODO : continue the loop until maximum texture limit reached (in order to clear the previous ones)
    */

    // TODO : texture binding in shader->Use() method

	for (std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>> mesh : m_meshes)
	{
        std::get<1>(mesh)->Draw(_view, _model * std::get<0>(mesh), _perspective);

        // TODO : in case the mesh isn't valid, delete it from the model
        // TODO : even better, store the mesh pointer inside a shared_ptr instead and the asset_manager can load/unload the mesh depending if it's needed or not
	}
}