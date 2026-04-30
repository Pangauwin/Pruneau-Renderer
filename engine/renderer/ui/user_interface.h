#pragma once

#include <string>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImGui {



bool EditableLabel(const char* id, std::string& value);
}