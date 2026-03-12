#pragma once

#include "../component.h"

#include "renderer/model.h"

#include "renderer/renderer.h"

#include <memory>

namespace Core
{

class ModelRenderer : public Component, AutoRegisterComponent<ModelRenderer>
{
public:
	explicit ModelRenderer(Entity* owner) : Component(owner), model(nullptr), _model_id(0) // Used by editor
	{
		Renderer::Renderer::Get()->RegisterModel(this);
	} 

	ModelRenderer(Entity* owner, AssetID _model); // Used for compiler to distinguish the one used by editor and the normal one
	~ModelRenderer();

	void OnEditorRender();

	const Renderer::Model* model; //TODO : Replace this pointer by a smart one
	AssetID _model_id;
private:

};

}