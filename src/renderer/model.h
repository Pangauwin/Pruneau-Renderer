#pragma once

#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <string>
#include <vector>

namespace Core
{
	class AssetManager;
}

namespace Renderer
{

class Model
{
	friend class Core::AssetManager;

public:
	Model();
	Model(Shader* _shader);
	Model(Shader* _shader, std::vector<Texture*>& _textures);
	~Model();

	void Draw();
	void SetShader(Shader* _shader);
	Shader* GetShader();

private:
	Shader* shader = nullptr;

private:
	std::vector<Mesh*> m_meshes;
	std::string directory;

	std::vector<Shader*> m_shaders;
	std::vector<Texture*> m_textures;
};

}