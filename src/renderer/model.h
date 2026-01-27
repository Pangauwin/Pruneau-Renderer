#pragma once

#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	~Model();
	//TODO : Work on an asset importer instead of calling assimp for 3d models

	void Draw();

public:
	Shader* shader;

private:

	std::vector<Mesh*> m_meshes;
	std::string directory;

	std::vector<Shader*> m_shaders;
};

}