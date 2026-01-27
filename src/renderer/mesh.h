#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

namespace Renderer {

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv_coord;

	Vertex(glm::vec3 _position = glm::vec3(), glm::vec3 _normal = glm::vec3(), glm::vec2 _uv_coord = glm::vec2()) : position(_position), normal(_normal), uv_coord(_uv_coord) {}
};

class Mesh {
public:
	Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, std::vector<Texture*> _textures);

	void AssignShader(Shader* _shader);

	~Mesh();
	void Draw();

public:
	Shader* shader;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture*> m_textures;

	unsigned int VAO, VBO, EBO;
};

}