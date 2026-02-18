#include "model_renderer.h"

#include "renderer/renderer.h"

namespace Core {

    static bool registered = []()
        {
            AutoRegisterComponent<ModelRenderer>::Register("Renderer");
            return true;
        }();


    ModelRenderer::ModelRenderer(Entity* _owner, Renderer::Model* _model) : Component(_owner), model(_model)
    {
        Renderer::Renderer::Get()->RegisterModel(this);
    }

    ModelRenderer::~ModelRenderer()
    {
        Renderer::Renderer::Get()->UnRegisterModel(this);
    }

}