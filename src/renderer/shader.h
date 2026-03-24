#pragma once

#include <string>

#include <glm/fwd.hpp>

namespace Renderer {

class Shader {
public:
	Shader(const char* _vertex_shader_code, const char* _fragment_shader_code);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetMat4(const std::string& _name, const glm::mat4& _val) const;
	void SetVec2(const std::string& _name, const glm::vec2& _val) const;
	void SetVec3(const std::string& _name, const glm::vec3& _val) const;
	void SetVec4(const std::string& _name, const glm::vec4& _val) const;
	void SetFloat(const std::string& _name, float _val) const;
	void SetInt(const std::string& _name, int _val) const;
	void SetBool(const std::string& _name, bool _val) const;

	const int& GetID() const {return m_id; }

	// TODO : Optimize Setters (can be expensive if called each frame)

private:
	unsigned int m_id;
};

}