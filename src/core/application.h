#pragma once

#include <memory>

#include "layer.h"
#include "layer_stack.h"

#include "../platform/window.h"

#include "../renderer/renderer.h"


namespace Core {

struct AppParams {
	Platform::WindowParams window_params;
};

class Application {
public:
	Application(AppParams _params);
	~Application();

	void Run();
	void OnEvent(Event& _event);
	void CloseApplication();

	void PushLayer(Layer* _layer);
	void PushOverlay(Layer* _layer);

	static Application* Get();

private:
	void PollEvents();
	float CalculateDeltaTime();

private:
	void OnClose();

private:
	std::unique_ptr<Platform::Window> m_window;
	std::unique_ptr<Renderer::Renderer> m_renderer;

	bool m_app_should_close;

	LayerStack m_layer_stack;

	EngineLayer::EngineLayer* m_engine_layer;
};

}