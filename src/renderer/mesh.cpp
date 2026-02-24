#include "mesh.h"

#include <vector>

#include <glad/glad.h>

Renderer::Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices):
	m_vertices(_vertices), m_indices(_indices)
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

Renderer::Mesh::~Mesh()
{
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

Renderer::Mesh* Renderer::Mesh::CreateTriangle()
{
    std::vector<Renderer::Vertex> vertices = {
        { { 0.0f,  0.5f, 0.0f }, {0,0,1}, {0.5f,1.0f} },
        { {-0.5f, -0.5f, 0.0f }, {0,0,1}, {0.0f,0.0f} },
        { { 0.5f, -0.5f, 0.0f }, {0,0,1}, {1.0f,0.0f} }
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    std::vector<Texture*> textures;

    return new Renderer::Mesh(vertices, indices, textures);
}

Renderer::Mesh* Renderer::Mesh::CreatePlane()
{
    std::vector<Renderer::Vertex> vertices = {
        { {-0.5f, 0.0f, -0.5f}, {0,1,0}, {0,0} },
        { { 0.5f, 0.0f, -0.5f}, {0,1,0}, {1,0} },
        { { 0.5f, 0.0f,  0.5f}, {0,1,0}, {1,1} },
        { {-0.5f, 0.0f,  0.5f}, {0,1,0}, {0,1} }
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<Texture*> textures;

    return new Renderer::Mesh(vertices, indices, textures);
}

Renderer::Mesh* Renderer::Mesh::CreateCube()
{

    std::vector<Renderer::Vertex> vertices = {
        // Front
        {{-0.5f,-0.5f, 0.5f},{0,0,1},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,0,1},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,0,1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{0,0,1},{0,1}},

        // Back
        {{ 0.5f,-0.5f,-0.5f},{0,0,-1},{0,0}},
        {{-0.5f,-0.5f,-0.5f},{0,0,-1},{1,0}},
        {{-0.5f, 0.5f,-0.5f},{0,0,-1},{1,1}},
        {{ 0.5f, 0.5f,-0.5f},{0,0,-1},{0,1}},

        // Left
        {{-0.5f,-0.5f,-0.5f},{-1,0,0},{0,0}},
        {{-0.5f,-0.5f, 0.5f},{-1,0,0},{1,0}},
        {{-0.5f, 0.5f, 0.5f},{-1,0,0},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{-1,0,0},{0,1}},

        // Right
        {{ 0.5f,-0.5f, 0.5f},{1,0,0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{1,0,0},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{1,0,0},{1,1}},
        {{ 0.5f, 0.5f, 0.5f},{1,0,0},{0,1}},

        // Top
        {{-0.5f, 0.5f, 0.5f},{0,1,0},{0,0}},
        {{ 0.5f, 0.5f, 0.5f},{0,1,0},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{0,1,0},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{0,1,0},{0,1}},

        // Bottom
        {{-0.5f,-0.5f,-0.5f},{0,-1,0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{0,-1,0},{1,0}},
        {{ 0.5f,-0.5f, 0.5f},{0,-1,0},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{0,-1,0},{0,1}},
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0,       // Front
        4,5,6, 6,7,4,       // Back
        8,9,10, 10,11,8,    // Left
        12,13,14, 14,15,12, // Right
        16,17,18, 18,19,16, // Top
        20,21,22, 22,23,20  // Bottom
    };

    std::vector<Texture*> textures;

    return new Renderer::Mesh(vertices, indices, textures);
}

void Renderer::Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}