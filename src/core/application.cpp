#include "application.h"

#include <memory>
#include <string>

#include "../platform/window.h"

#include "level_manager.h"

#include "layer.h"
#include "../layers/engine_layer.h"
#include "../renderer/renderer.h"

#include "core/time.h"

#include "asset/asset_manager.h"

#include <iostream>

static float dt = 0.00001f; // Avoid 0 divisions errors

static Core::Application* current_application;

void Core::LogMessage(std::string _message)
{
	if(Core::Application::Get())
		Core::Application::Get()->LogMessage(_message, EngineLayer::LOG_PRIORITY_NONE);

	std::cout << "[MESSAGE] : " << _message.c_str() << "\n";
}

void Core::LogMessageDebug(std::string _message)
{
	if(Core::Application::Get())
		Core::Application::Get()->LogMessage(_message, EngineLayer::LOG_PRIORITY_DEBUG);

	std::cout << "[DEBUG] : " << _message.c_str() << "\n";
}

void Core::LogMessageInfo(std::string _message)
{
	if(Core::Application::Get())
		Core::Application::Get()->LogMessage(_message, EngineLayer::LOG_PRIORITY_INFO);

	std::cout << "[INFO]" << _message.c_str() << "\n";
}

void Core::LogMessageWarning(std::string _message)
{
	if(Core::Application::Get())
		Core::Application::Get()->LogMessage(_message, EngineLayer::LOG_PRIORITY_WARNING);

	std::cout << "[WARNING]" << _message.c_str() << "\n";
}

void Core::LogMessageError(std::string _message)
{
	if(Core::Application::Get())
		Core::Application::Get()->LogMessage(_message, EngineLayer::LOG_PRIORITY_ERROR);

	std::cout << "[ERROR]" << _message.c_str() << "\n";
}

Core::Application::Application(AppParams _params) : 
	m_window(std::make_unique<Platform::Window>(_params.window_params)), 
	m_renderer(std::make_unique<Renderer::Renderer>(m_window.get())), 
	m_engine_layer(new EngineLayer::EngineLayer()),
	m_app_should_close(false)
{
	current_application = this;

	new LevelManager();
}

Core::Application::~Application()
{

}

void Core::Application::Init()
{
	AssetManager::Init();
	Time::Init();

	PushOverlay(m_engine_layer);
}

void Core::Application::Run()
{
	Init();

	while (!m_app_should_close)
	{
		Time::Update();

		PollEvents();
		m_window->SwapBuffers();

		dt = Time::delta_time;

		for (Layer* _layer : m_layer_stack)
			_layer->OnUpdate(dt);

		LevelManager::OnUpdate(dt);

		m_renderer->PreRender();

		LevelManager::OnRender();

		for (Layer* _layer : m_layer_stack)
			_layer->OnRender();

		m_renderer->PostRender();

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

void Core::Application::LogMessage(std::string _message, EngineLayer::LOG_PRIORITY _priority)
{
	m_engine_layer->LogMessage(_message, _priority);
}

void Core::Application::PollEvents()
{
	m_window->PollEvents();
}

void Core::Application::OnClose()
{
	for (Layer* _layer : m_layer_stack)
		_layer->OnDetach();

	delete m_renderer.release();
	delete m_window.release();
}
