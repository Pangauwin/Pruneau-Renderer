#pragma once

#include <memory>
#include <vector>

#include "layer.h"


namespace Core {

class LayerStack {
public:
	~LayerStack();

	void PushLayer(Layer* _layer);
	void PushOverlay(Layer* _overlay);
	void PopLayer(Layer* _layer);
	void PopOverlay(Layer* _overlay);

	auto begin() { return m_layers.begin(); }
	auto end() { return m_layers.end(); }

private:
	std::vector<Layer*> m_layers;
	unsigned int m_layer_insert_index = 0;
};

}