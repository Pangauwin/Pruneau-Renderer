#pragma once

#include "../../src/core/layer.h"
#include "../core/event/event.h"

#include <vector>
#include <string>

namespace EngineLayer {

class EngineLayer : public Core::Layer
{
public:
	EngineLayer() : Layer("EngineLayer") {}

	void OnAttach() override;
	void OnGUIRender() override;
	void OnEvent(Core::Event& _event) override;

	void LogMessage(std::string _message);
	// TODO : Create a clear function

private:
	std::vector<std::string> m_message_pool; // TODO : Create a message type in order to store more information about the message (error, warning, etc...)
};

}