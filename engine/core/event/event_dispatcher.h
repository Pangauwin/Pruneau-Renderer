#pragma once

#include "event.h"

namespace Core {

class EventDispatcher {
public:
	EventDispatcher(Event& event) : m_event(event) {}

	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_event.GetEventType() == T::GetStaticType())
		{
			m_event.handled = func(static_cast<T&>(m_event));
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};

}