#pragma once

#include "../../src/core/layer.h"
#include "../core/event/event.h"

#include <vector>
#include <string>
#include <tuple>

namespace EngineLayer {

enum LOG_PRIORITY {
	LOG_PRIORITY_NONE = 0,

	LOG_PRIORITY_DEBUG,
	LOG_PRIORITY_INFO,
	LOG_PRIORITY_WARNING,
	LOG_PRIORITY_ERROR,
};

class EngineLayer : public Core::Layer
{
public:
	EngineLayer() : Layer("EngineLayer") {}

	void OnAttach() override;
	void OnGUIRender() override;
	void OnEvent(Core::Event& _event) override;

	void LogMessage(std::string _message, LOG_PRIORITY _priority);
	void ClearConsole();

	// TODO : Create a editor layout contains the editor features (such as drop file in the OnEvent() function etc...)

private:
	std::vector<std::tuple<LOG_PRIORITY, std::string>> m_message_pool;
};

}