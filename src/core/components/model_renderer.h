#pragma once

#include "../component.h"

#include "renderer/model.h"

namespace Core
{

class ModelRenderer : public Component
{
public:
	ModelRenderer();
	~ModelRenderer();

	Renderer::Model* model;

private:

};

}