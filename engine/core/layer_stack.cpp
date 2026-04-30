#include "layer_stack.h"

Core::LayerStack::~LayerStack()
{
	for (Layer* _layer : m_layers)
	{
		_layer->OnDetach();
		delete _layer;
	}
}

void Core::LayerStack::PushLayer(Layer* _layer)
{
	m_layers.emplace(m_layers.begin() + m_layer_insert_index, _layer);
	m_layer_insert_index++;
	_layer->OnAttach();
}

void Core::LayerStack::PushOverlay(Layer* _overlay)
{
	m_layers.emplace_back(_overlay);
	_overlay->OnAttach();
}

void Core::LayerStack::PopLayer(Layer* _layer)
{
}

void Core::LayerStack::PopOverlay(Layer* _overlay)
{
}
