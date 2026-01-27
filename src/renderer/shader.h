#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Renderer {

class Shader {
public:
	Shader(const char* _vertex_shader_path, const char* _fragment_shader_path);
	~Shader();

	void Use() const;
	void UnBind() const;

	void SetMat4(std::string _name, glm::mat4& _val);
	void SetFloat(std::string _name, float _val);
	void SetInt(std::string _name, int _val);
	void SetBool(std::string _name, bool _val);

private:
	unsigned int m_id;
};

}