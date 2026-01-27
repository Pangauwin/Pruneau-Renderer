// TODO : Move window close event (in event.h file) here
#include "../event.h"

class WindowCloseEvent : public Core::Event
{
public:
	static Core::event_type GetStaticType() { return Core::event_type::event_type_window_close; }
	Core::event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "WindowClose"; }
	int GetCategoryFlags() const override { return Core::event_category_application; }
};