#include "../event.h"

namespace Core 
{

class CharEvent : public Event
{
public:
	CharEvent(int _codepoint) : codepoint(_codepoint) {}

	static event_type GetStaticType() { return event_type::event_type_keyboard; }
	event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "Char"; }
	int GetCategoryFlags() const override { return event_category_keyboard | event_category_input; }

public:
	int codepoint;
};

}