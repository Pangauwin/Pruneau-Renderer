#include "user_interface.h"
#include "imgui.h"

bool ImGui::EditableLabel(const char* _id, std::string &value)
{
    ImGuiID id = ImGui::GetID(_id);

    static ImGuiID active_id = -1;
    static char buffer[255];

    bool changed = false;

    if(active_id == id)
    {
        ImGui::SetNextItemWidth(ImGui::CalcTextSize(buffer).x + 10.0f);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0,0,0,0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));

        ImGui::SetKeyboardFocusHere();
    
        bool enter_pressed = InputText("##edit", buffer, sizeof(buffer),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

        PopStyleVar();
        PopStyleColor(2);

        if(enter_pressed)
        {
            value = buffer;
            active_id = -1;
            changed = true;
        }

        if(IsKeyPressed(ImGuiKey_Escape))
        {
            active_id = -1;
        }

        if(ImGui::IsItemDeactivated())
        {
            active_id = -1;
        }
    }

    else {
        TextUnformatted(value.c_str());

        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            active_id = id;
            strncpy(buffer, value.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';
        }
    }

    return changed;

}