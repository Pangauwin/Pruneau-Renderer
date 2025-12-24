#include "engine_layer.h"

#include <imgui.h>
#include <string>

#include "../core/application.h"

#define CONSOLE_COMMAND_MAX_SIZE 512

static char command[CONSOLE_COMMAND_MAX_SIZE];

void EngineLayer::EngineLayer::OnAttach()
{
	
}

void EngineLayer::EngineLayer::OnGUIRender()
{
    static bool dockspaceOpen = true;
    static bool optFullscreen = true;

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking;

    if (optFullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }

    ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);

    if (optFullscreen)
        ImGui::PopStyleVar(2);

    ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                Core::Application::Get()->CloseApplication();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

	ImGui::Begin("Console");

    for (std::string message : m_message_pool)
    {
        ImGui::Text(message.c_str());
    }

    if (ImGui::InputText("command", command, CONSOLE_COMMAND_MAX_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        // TODO : Interpret the command and log the result instead of the command
        LogMessage(command);
    }

    ImGui::SetScrollHereY(1.0f);

	ImGui::End();
}

void EngineLayer::EngineLayer::OnEvent(Core::Event& _event)
{
	if (_event.IsInCategory(Core::event_category_input))
	{
		ImGuiIO& io = ImGui::GetIO();
		_event.handled |= io.WantCaptureKeyboard || io.WantCaptureMouse;
	}
}

void EngineLayer::EngineLayer::LogMessage(std::string _message)
{
    m_message_pool.push_back(_message);
}
