#pragma once

#include "../component.h"

#include "renderer/model.h"

namespace Core
{

class ModelRenderer : public Component, AutoRegisterComponent<ModelRenderer>
{
public:
	explicit ModelRenderer(Entity* owner) : Component(owner), model(nullptr) // Used by editor
	{

	} 

	ModelRenderer(Entity* owner, Renderer::Model* _model); // Used for compiler to distinguish the one used by editor and the normal one
	~ModelRenderer();

	Renderer::Model* model;

private:

};

}