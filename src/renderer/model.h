#pragma once

#include "core/asset/asset.h"

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
	Model(std::vector<std::shared_ptr<Core::MeshAsset>> _meshes);
	~Model() = default;

	void Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective);
private:
	std::vector<std::shared_ptr<Core::MeshAsset>> m_meshes;

	// Add the transform of the meshes present inside the model
};

}