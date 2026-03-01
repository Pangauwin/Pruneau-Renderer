#pragma once

#include "core/asset/asset.h"

#include <vector>
#include <memory>
#include <tuple>

#include <glm/fwd.hpp>

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
	Model(std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes);
	~Model() = default;

	void Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective);
private:
	std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> m_meshes;
};

}