#include "model_renderer.h"

#include "renderer/renderer.h"

#include "core/asset/asset_manager.h"

#include <imgui.h>

namespace Core {

static bool registered = []()
    {
        AutoRegisterComponent<ModelRenderer>::Register("Model Renderer");
        return true;
    }();


ModelRenderer::ModelRenderer(Entity* _owner, AssetID _model) : Component(_owner), _model_id(_model), model(nullptr)
{
    if (auto model_asset = AssetManager::GetAsset<ModelAsset>(_model_id))
    {
        model = model_asset->GetModel();
    }
    Renderer::Renderer::Get()->RegisterModel(this);
}

ModelRenderer::~ModelRenderer()
{
    Renderer::Renderer::Get()->UnRegisterModel(this);
}

void ModelRenderer::OnEditorRender()
{
    int _id = _model_id;

    if (ImGui::DragInt("Model ID: ", &_id) && _id > 0)
    {
        _model_id = _id;

        if (auto model_asset = AssetManager::GetAsset<ModelAsset>(_model_id))
        {
            model = model_asset->GetModel();
        }
    }
}
}