#include "application.h"

#include <memory>
#include <string>

#include "../platform/window.h"

#include "level_manager.h"

#include "layer.h"
#include "../layers/engine_layer.h"
#include "../renderer/renderer.h"

static float dt = 0.00001f;
static float time = 0.0f;

static Core::Application* current_application;

void Core::LogMessage(std::string _message)
{
	Core::Application::Get()->LogMessage(_message);
}

Core::Application::Application(AppParams _params) : 
	m_window(std::make_unique<Platform::Window>(_params.window_params)), 
	m_renderer(std::make_unique<Renderer::Renderer>(m_window.get())), 
	m_engine_layer(new EngineLayer::EngineLayer()),
	m_app_should_close(false)
{
	current_application = this;

	new LevelManager();

	PushOverlay(m_engine_layer);
}

Core::Application::~Application()
{

}

void Core::Application::Run()
{
	while (!m_app_should_close)
	{
		m_window->SwapBuffers();

		PollEvents();

		dt = CalculateDeltaTime();

		for (Layer* _layer : m_layer_stack)
			_layer->OnUpdate(dt);

		LevelManager::OnUpdate(dt);

		m_renderer->PreRender();

		LevelManager::OnRender();

		for (Layer* _layer : m_layer_stack)
			_layer->OnRender();

		m_renderer->PostRender();

		/*
		TODO : For Game-Releases builds :
			- Implement EditorRender() instead of rendering the editor during GUIRender()
		*/

		m_renderer->PreGUIRender();

		for (Layer* _layer : m_layer_stack)
			_layer->OnGUIRender();

		LevelManager::OnGUIRender();

		m_renderer->PostGUIRender();
	}

	OnClose();
}

void Core::Application::OnEvent(Event& _event)
{
	/*
	Exemple of event dispatching (put this in the layer code)

	#include "core/event/event_dispatcher.h"

	EventDispatcher dispatcher(_event);

	dispatcher.Dispatch<Platform::WindowCloseEvent>([this](Platform::WindowCloseEvent&) {
		m_app_should_close = true;
	});*/

	for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
	{
		(*--it)->OnEvent(_event);
		if (_event.handled)
			break;
	}
}

void Core::Application::CloseApplication()
{
	m_app_should_close = true;
}

void Core::Application::PushLayer(Layer* _layer)
{
	m_layer_stack.PushLayer(_layer);
}

void Core::Application::PushOverlay(Layer* _layer)
{
	m_layer_stack.PushOverlay(_layer);
}

Core::Application* Core::Application::Get()
{
	return current_application;
}

void Core::Application::LogMessage(std::string _message)
{
	m_engine_layer->LogMessage(_message);
}

void Core::Application::PollEvents()
{
	m_window->PollEvents();
}

float Core::Application::CalculateDeltaTime()
{
	float previous_time = time;
	time = m_window.get()->GetTime();
	dt = time - previous_time;
	return dt;
}

void Core::Application::OnClose()
{
	for (Layer* _layer : m_layer_stack)
		_layer->OnDetach();

	// TODO : destroy window
}
