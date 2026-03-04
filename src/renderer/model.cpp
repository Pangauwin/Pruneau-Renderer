#include "model.h"

#include <glm/glm.hpp>

Renderer::Model::Model(std::vector<std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>>> _meshes) : m_meshes(std::move(_meshes)) {}

void Renderer::Model::Draw(const glm::mat4& _view, const glm::mat4& _model, const glm::mat4& _perspective)
{
	for (std::tuple<glm::mat4, std::shared_ptr<Core::MeshAsset>> mesh : m_meshes)
	{
        std::get<1>(mesh)->Draw(_view, _model * std::get<0>(mesh), _perspective);

        // TODO : in case the mesh isn't valid, delete it from the model
        // TODO : even better, store the mesh pointer inside a shared_ptr instead and the asset_manager can load/unload the mesh depending if it's needed or not
	}
}