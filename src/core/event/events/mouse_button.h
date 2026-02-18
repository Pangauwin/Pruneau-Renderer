#include "../event.h"

namespace Core
{

class MouseButtonEvent : public Event
{
public:
	MouseButtonEvent(int _button, int _action, int _mods) : button(_button), action(_action), mods(_mods) {}

	static event_type GetStaticType() { return event_type::event_type_mouse_button; }
	event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "MouseButton"; }
	int GetCategoryFlags() const override { return event_category_input; }

public:
	int button, action, mods;
};

}