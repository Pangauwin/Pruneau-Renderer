#include "../event.h"

namespace Core 
{

class KeyboardEvent : public Event
{
public:
	KeyboardEvent(int _key, int _scancode, int _action, int _mods) : key(_key), scancode(_scancode), action(_action), mods(_mods) {}

	static event_type GetStaticType() { return event_type::event_type_keyboard; }
	event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "Keyboard"; }
	int GetCategoryFlags() const override { return event_category_keyboard | event_category_input; }

public:
	int key, scancode, action, mods;
};

}