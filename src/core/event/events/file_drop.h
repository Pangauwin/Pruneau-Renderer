#pragma once

#include "../event.h"

#include <vector>
#include <string>

namespace Core {

class FileDropEvent : public Event
{
public:
	FileDropEvent(std::vector<std::string> _paths) : m_paths(_paths) {}

	const std::vector<std::string>& GetPaths() const { return m_paths; }

	static event_type GetStaticType() { return event_type::event_type_file_dropped; }
	event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "FileDrop"; }
	int GetCategoryFlags() const override { return event_category_input; }

private:
	std::vector<std::string> m_paths;
};

}