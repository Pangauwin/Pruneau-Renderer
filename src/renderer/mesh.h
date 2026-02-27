#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

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
	Mesh(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices, std::weak_ptr<Shader> _shader);
	~Mesh();

	/*static Mesh* CreateTriangle();
	static Mesh* CreatePlane();
	static Mesh* CreateCube();*/

	// TODO : recreate these functions

	void Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective);

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	std::weak_ptr<Shader> m_shader;

	unsigned int VAO, VBO, EBO;
};

}