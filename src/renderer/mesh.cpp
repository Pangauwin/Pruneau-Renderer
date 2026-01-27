#include "mesh.h"

#include <vector>

#include <glad/glad.h>

Renderer::Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, std::vector<Renderer::Texture*> _textures):
	m_vertices(_vertices), m_indices(_indices), m_textures(_textures)
{

#pragma region OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv_coord));

	glBindVertexArray(0);
#pragma endregion

}

void Renderer::Mesh::AssignShader(Shader* _shader)
{
    shader = _shader;
}

Renderer::Mesh::~Mesh()
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Renderer::Mesh::Draw()
{
    if (shader == nullptr) return;

    unsigned int diffuse_nb = 0;
    unsigned int specular_nb = 0;
    shader->Use();

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
            specular_nb++;
            name = "texture_normal_" + std::to_string(specular_nb);
            break;
        default:
            break;
        }

        shader->SetInt(name.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetID());
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}