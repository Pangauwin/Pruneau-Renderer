#pragma once

#include "../../src/core/layer.h"
#include "../core/event/event.h"

namespace EngineLayer {

class EngineLayer : public Core::Layer
{
public:
	EngineLayer() : Layer("EngineLayer") {}

	void OnAttach() override;
	void OnGUIRender() override;
	void OnEvent(Core::Event& _event) override;
};

}