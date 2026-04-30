#pragma once

#include "event/event.h"

namespace Core {

class Layer
{
public:
	explicit Layer(const char* _name = "Layer") : m_name(_name) {}

	virtual ~Layer() = default;
	
	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void OnEvent(Event& _event) {}

	virtual void OnUpdate(float dt) {}
	virtual void OnRender() {}
	virtual void OnGUIRender() {}

	const char* GetName() const { return m_name; }

protected:
	const char* m_name;
};

}