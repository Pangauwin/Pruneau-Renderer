#pragma once

#include "shader.h"
#include "mesh.h"

#include <vector>
#include <memory>

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
	Model(std::vector<std::weak_ptr<Mesh>> _meshes);
	~Model() = default;

	void Draw();
private:
	std::vector<std::weak_ptr<Mesh>> m_meshes;

	// Add the transform of the meshes present inside the model
};

}