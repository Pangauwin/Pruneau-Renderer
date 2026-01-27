#include "engine_layer.h"

#include <imgui.h>
#include <string>
#include <filesystem>

#include "../core/application.h"
#include "../core/event/event_dispatcher.h"

#include "core/event/events/file_drop.h"

#include "core/asset/asset_manager.h"

#include "core/level_manager.h"

#define CONSOLE_COMMAND_MAX_SIZE 512

static char command[CONSOLE_COMMAND_MAX_SIZE];

static ImTextureID file_icon = 0;

static Core::Entity* selected_entity = nullptr;

static void DrawEntityNode(Core::Entity* _entity);

void EngineLayer::EngineLayer::OnAttach()
{
	
}

void EngineLayer::EngineLayer::OnGUIRender()
{
#pragma region Dockspace
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
#pragma endregion

#pragma region MenuBar

    if (ImGui::BeginMenuBar())
    {
        // TODO : Implement MenuBar features

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {

            }

            if (ImGui::MenuItem("Load"))
            {

            }

            if (ImGui::MenuItem("Exit"))
            {
                Core::Application::Get()->CloseApplication();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Project Settings"))
            {

            }

            if (ImGui::MenuItem("Options"))
            {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
#pragma endregion

#pragma region Console

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
#pragma endregion

#pragma region AssetExplorer
    ImGui::Begin("Asset Explorer");

    ImGui::BeginGroup();

    for each(const Core::Asset * _asset in Core::AssetManager::GetAssets())
    {
        if (ImGui::Button(_asset->name.c_str(), ImVec2(64, 64)))
        {
            // File clicked
            LogMessage("DID YOU CLICK ME LA OH !!!");
        }

        /*if (ImGui::ImageButton(_asset->name.c_str(), file_icon, ImVec2(64, 64), ImVec2(0, 0), ImVec2(1, 1)))
        {
            // File clicked
            LogMessage("DID YOU CLICK ME LA OH !!!");
        }*/
    }

    ImGui::EndGroup();

    ImGui::End();

#pragma endregion

#pragma region LevelInspector
    ImGui::Begin("Level Inspector");

    Core::Level* _level = Core::LevelManager::GetCurrentLevel();

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Button("Add Entity"))
        {
            _level->CreateEntity("Entity", nullptr); // TODO : fix (broken)
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Delete"))
        {
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Rename"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    
    ImGui::BulletText(_level->name.c_str());

    // TODO : Instead of doing that on the runtime, cache the parent/child and call update_tree functions when needed (Entity created/destroyed)
    for (auto& _entity : _level->entities)
    {
        if (_entity->parent == nullptr)
        {
            DrawEntityNode(_entity);
        }
    }

    ImGui::End();
#pragma endregion

#pragma region Inspector
    ImGui::Begin("Inspector");

    if (selected_entity)
    {
        ImGui::BulletText(selected_entity->name.c_str());

        for (auto& i : selected_entity->components)
        {
            std::string comp_name = typeid(*i.second.get()).name();

            std::string result = comp_name.substr(comp_name.find_last_of(":") + 1);

            ImGui::BeginChild(result.c_str());

            bool enabled = true; // TODO : Implement Component enable
            ImGui::Checkbox("##enabled", &enabled);

            ImGui::SameLine();

            if (ImGui::CollapsingHeader(result.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                i.second.get()->OnEditorRender();
            }
            
            ImGui::EndChild();
        }
        // TODO : same here
        if (ImGui::BeginPopupContextWindow())
        {
            ImGui::Button("Add Component");

            ImGui::EndPopup();
        }
    }

    ImGui::End();
#pragma endregion
}

void EngineLayer::EngineLayer::OnEvent(Core::Event& _event)
{
    Core::EventDispatcher dispatcher(_event);

    dispatcher.Dispatch<Core::FileDropEvent>(
        [this](Core::FileDropEvent& event)
        {
            bool state = true;

            for (const auto& path : event.GetPaths())
            {
                std::filesystem::path p(path);

                if (p.extension() == ".fbx")
                {
                    Core::AssetManager::ImportAsset(path.c_str());
                    LogMessage("Asset imported : " + path);
                }
                else state = false;
            }

            return state;
        }
    );

    // TODO : FIX THIS !!!!! We should check if the input is keyboard or mouse before handling it ! Here it also intercepts events such as file drops, etc...
    // Actually, it intercepts all events marked as Core::event_category_input
    // We should also modify the condition as we want input inside the scene window (holding the framebuffer)
	if (_event.IsInCategory(Core::event_category_input))
	{
		ImGuiIO& io = ImGui::GetIO();
		_event.handled |= io.WantCaptureKeyboard || io.WantCaptureMouse;

        if (_event.handled)
            return;
	}
}

void EngineLayer::EngineLayer::LogMessage(std::string _message)
{
    m_message_pool.push_back(_message);
}


static void DrawEntityNode(Core::Entity* _entity)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (_entity->children.size() == 0)
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool open = ImGui::TreeNodeEx((void*)_entity, flags, "%s", _entity->name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        selected_entity = _entity;
    }

    if (open)
    {
        for (Core::Entity* e : _entity->children)
        {
            DrawEntityNode(e);
        }

        ImGui::TreePop();
    }
}