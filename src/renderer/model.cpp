#include "model.h"

#include <glad/glad.h>

Renderer::Model::Model() : shader(nullptr)
{

}

Renderer::Model::Model(Shader* _shader) : shader(_shader)
{

}

Renderer::Model::Model(Shader* _shader, std::vector<Texture*>& _textures) : shader(_shader), m_textures(_textures)
{

}

Renderer::Model::~Model()
{
	for (Mesh* mesh : m_meshes)
	{
		delete mesh;
	}
}

void Renderer::Model::Draw()
{
    unsigned int diffuse_nb = 0;
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
        */

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


	for (Mesh* mesh : m_meshes)
	{
		mesh->Draw();
	}
}

void Renderer::Model::SetShader(Shader* _shader)
{
	shader = _shader;
}

Renderer::Shader* Renderer::Model::GetShader()
{
	return shader;
}
