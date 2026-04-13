#include "../event.h"

namespace Core 
{

class CursorPosition : public Event
{
public:
	CursorPosition(double _xpos, double _ypos) : xpos(_xpos), ypos(_ypos) {}

	static event_type GetStaticType() { return event_type::event_type_mouse_moved; }
	event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "CursorPosition"; }
	int GetCategoryFlags() const override { return event_category_mouse_motion | event_category_input; }

public:
	double xpos, ypos;
};

}