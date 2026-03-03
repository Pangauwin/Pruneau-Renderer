#include "shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Shader::Shader(const char* _vertex_shader_code, const char* _fragment_shader_code) : m_id(0)
{
	unsigned int vertex_id, fragment_id;
	int success;

	char info_log[512];

	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &_vertex_shader_code, NULL);
	glCompileShader(vertex_id);

	glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_id, 512, NULL, info_log);
		// TODO : Log error
		glDeleteShader(vertex_id);
		return;
	}

	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &_fragment_shader_code, NULL);
	glCompileShader(fragment_id);

	glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_id, 512, NULL, info_log);
		glDeleteShader(vertex_id);
		glDeleteShader(fragment_id);
		return;
	}

	m_id = glCreateProgram();

	glAttachShader(m_id, vertex_id);
	glAttachShader(m_id, fragment_id);

	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, info_log);

		//TODO : Log Errors
		glDeleteProgram(m_id);
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

Renderer::Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Renderer::Shader::Bind() const
{
	glUseProgram(m_id);
}

void Renderer::Shader::UnBind() const
{
	glUseProgram(0);
}

void Renderer::Shader::SetVec2(const std::string& _name, const glm::vec2& _val) const
{
	Bind();
	glUniform2f(glGetUniformLocation(m_id, _name.c_str()), _val.x, _val.y);
}

void Renderer::Shader::SetVec3(const std::string& _name, const glm::vec3& _val) const
{
	Bind();
	glUniform3f(glGetUniformLocation(m_id, _name.c_str()), _val.x, _val.y, _val.z);
}

void Renderer::Shader::SetVec4(const std::string& _name, const glm::vec4& _val) const
{
	Bind();
	glUniform4f(glGetUniformLocation(m_id, _name.c_str()), _val.x, _val.y, _val.z, _val.t);
}

void Renderer::Shader::SetMat4(const std::string& _name,const glm::mat4& _val) const
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(m_id, _name.c_str()), 1, GL_FALSE, glm::value_ptr(_val));
}

void Renderer::Shader::SetFloat(const std::string& _name, float _val) const
{
	Bind();
	glUniform1f(glGetUniformLocation(m_id, _name.c_str()), _val);
}

void Renderer::Shader::SetInt(const std::string& _name, int _val) const
{
	Bind();
	glUniform1i(glGetUniformLocation(m_id, _name.c_str()), _val);
}

void Renderer::Shader::SetBool(const std::string& _name, bool _val) const
{
	Bind();
	glUniform1i(glGetUniformLocation(m_id, _name.c_str()), (int)_val);
}
