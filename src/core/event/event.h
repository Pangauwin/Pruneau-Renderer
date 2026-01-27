#pragma once

namespace Core {

enum class event_type {
	event_type_none = 0,

	event_type_window_close,
	event_type_window_resize,

	event_type_key_pressed,
	event_type_key_released,

	event_type_mouse_button_pressed,
	event_type_mouse_button_released,

	event_type_mouse_moved,

	event_type_file_dropped
};

enum event_category {
	event_category_none			= 0,
	event_category_application	= 1 << 0,
	event_category_input		= 1 << 1,
	event_category_keyboard		= 1 << 2,
	event_category_mouse		= 1 << 3,
	event_category_mouse_button	= 1 << 4,
};

class Event {
public:
	bool handled = false;

	virtual event_type GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;

	bool IsInCategory(event_category _category) const
	{
		return GetCategoryFlags() & _category;
	}
};

}