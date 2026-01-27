#include "model.h"

Renderer::Model::Model()
{

}

Renderer::Model::~Model()
{
	for each(Mesh* mesh in m_meshes)
	{
		delete mesh;
	}
}

void Renderer::Model::Draw()
{

}
