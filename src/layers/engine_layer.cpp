#include "engine_layer.h"

#include <imgui.h>
#include <memory>
#include <string>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <variant>
#include <cxxabi.h>

#include "../core/application.h"
#include "renderer/renderer.h"
#include "../core/event/event_dispatcher.h"

#include "renderer/texture.h"

#include "core/event/events/file_drop.h"

#include "core/asset/asset_manager.h"

#include "core/level_manager.h"

#include <ImGuizmo.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#include "core/time.h"

#include "renderer/ui/user_interface.h"
//TODO : clean other ImGui/ImGuizmo includes

#pragma region DEFINES

#define CONSOLE_COMMAND_MAX_SIZE 512

static char command[CONSOLE_COMMAND_MAX_SIZE];

using UISelectObject = std::variant<std::monostate, Core::Entity*, Core::AssetID>;

UISelectObject selected_object;

#pragma region Icons

struct EditorIcons {
    ImTextureID folder_icon;
    ImTextureID file_icon;
    ImTextureID texture_icon;
    ImTextureID model_icon;
};

static EditorIcons icons;

#pragma endregion


static ImGuizmo::OPERATION imguizmo_operation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE imguizmo_mode(ImGuizmo::LOCAL);

#pragma region Functions
static void DrawEntityNode(Core::Entity* _entity);

template<typename T>
static std::string GetTypeName(T& obj);
#pragma endregion

#pragma endregion

struct AssetExplorerState {
    Core::FolderID _current_folder = 0;
};

AssetExplorerState _state;

void EngineLayer::EngineLayer::OnAttach()
{
    Core::AssetID file_icon_asset = Core::AssetManager::ImportAsset("ressources/icons/file.png");
    Core::AssetID folder_icon_asset = Core::AssetManager::ImportAsset("ressources/icons/folder.png");

    icons.file_icon = Core::AssetManager::GetAsset<Core::TextureAsset>(file_icon_asset)->GetTexture()->GetID();
    icons.model_icon = Core::AssetManager::GetAsset<Core::TextureAsset>(file_icon_asset)->GetTexture()->GetID();
    icons.texture_icon = Core::AssetManager::GetAsset<Core::TextureAsset>(file_icon_asset)->GetTexture()->GetID();
    icons.folder_icon = Core::AssetManager::GetAsset<Core::TextureAsset>(folder_icon_asset)->GetTexture()->GetID();
}

void EngineLayer::EngineLayer::OnGUIRender()
{
    Core::Level* _level = Core::LevelManager::GetCurrentLevel();


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

    if (ImGui::Button("Clear")) ClearConsole();

    if (ImGui::InputText("command", command, CONSOLE_COMMAND_MAX_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        // TODO : Interpret the command and log the result instead of the command
        Core::LogMessage(command);
    }

    ImGui::BeginChild("Console Messages", ImVec2(0.0f, 0.0f));

    for (std::tuple<LOG_PRIORITY, std::string>& message : m_message_pool)
    {
        LOG_PRIORITY _priority = std::get<LOG_PRIORITY>(message);

        if(_priority == LOG_PRIORITY_NONE)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

        else if (_priority == LOG_PRIORITY_DEBUG)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 102, 0, 255));

        else if (_priority == LOG_PRIORITY_INFO)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(102, 178, 255, 255));

        else if (_priority == LOG_PRIORITY_WARNING)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));

        else if (_priority == LOG_PRIORITY_ERROR)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(204, 0, 0, 255));

        ImGui::Text("%s", std::get<std::string>(message).c_str());

        ImGui::PopStyleColor();
    }

    ImGui::EndChild();

	ImGui::End();
#pragma endregion

#pragma region AssetExplorer

    ImGui::Begin("Asset Explorer");

    Core::AssetFolder& _current_folder = Core::AssetManager::GetFolder(_state._current_folder);

    for (Core::AssetFolder* _folder : _current_folder.children)
    {
        if (ImGui::ImageButton(std::to_string(_folder->id).c_str(), icons.folder_icon, { 16.0f, 16.0f }))
        {
            _state._current_folder = _folder->id;
        }
    }

    if(ImGui::BeginTable("Explorer Table", 3, 
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Sortable |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_SizingStretchProp
    ))
    {
        ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("AssetID");

        for (Core::AssetFolder* _folder : _current_folder.children)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Image(icons.file_icon, ImVec2(32, 32));
        
            ImGui::TableSetColumnIndex(1);
            ImGuiSelectableFlags flags =
                ImGuiSelectableFlags_SpanAllColumns |
                ImGuiSelectableFlags_AllowOverlap;
            
            if(ImGui::Selectable(_current_folder.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, 32.0f)))
            {
                selected_object = _current_folder.id;
            }

            ImGui::TableSetColumnIndex(2);

            ImGui::TextUnformatted(std::to_string(_current_folder.id).c_str());
        }

        for (std::shared_ptr<Core::Asset> _asset : _current_folder.assets) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Image(icons.file_icon, ImVec2(32, 32));
        
            ImGui::TableSetColumnIndex(1);
            ImGuiSelectableFlags flags =
                ImGuiSelectableFlags_SpanAllColumns |
                ImGuiSelectableFlags_AllowOverlap;
            
            if(ImGui::Selectable(_asset->name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, 32.0f)))
            {
                selected_object = _asset->GetID();
            }

            ImGui::TableSetColumnIndex(2);

            ImGui::TextUnformatted(std::to_string(_asset->GetID()).c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();

#pragma endregion

#pragma region LevelInspector
    ImGui::Begin("Level Inspector");

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Button("Add Entity"))
        {
            _level->CreateEntity("Entity", nullptr);
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Delete"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EditableLabel("##level_name", _level->name);

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

#pragma region Properties
    ImGui::Begin("Properties");

    std::visit([](auto&& arg)
    {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Core::Entity*>)
            {
                if (arg)
                {
                    Core::Entity* selected_entity = arg;
                    ImGui::BulletText("%s", selected_entity->name.c_str());

                    for (auto& i : selected_entity->components)
                    {
                        std::string comp_name = GetTypeName(*i.second.get());

                        std::string result = comp_name.substr(comp_name.find_last_of(":") + 1);

                        ImGui::BeginGroup();

                        bool enabled = true; // TODO : Implement Component enable
                        ImGui::Checkbox("##enabled", &enabled);

                        ImGui::SameLine();

                        if (ImGui::CollapsingHeader(result.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            i.second.get()->OnEditorRender();
                        }

                        ImGui::EndGroup();
                    }

                    if (ImGui::Button("Add Component"))
                        ImGui::OpenPopup("AddComponentPopup");

                    if (ImGui::BeginPopup("AddComponentPopup"))
                    {
                        for (const auto& [type, info] : Core::ComponentRegistry::All())
                        {
                            if (ImGui::MenuItem(info.name.c_str()) && !(selected_entity->components[type]))
                            {
                                selected_entity->components[type] = info.factory(selected_entity);
                                ImGui::CloseCurrentPopup();
                            }
                        }

                        ImGui::EndPopup();
                    }
                }

                else
                {
                    ImGui::BulletText("Not valid Entity selected");
                }
            }

            else if constexpr (std::is_same_v<T, Core::AssetID>)
            {
                std::shared_ptr<Core::Asset> _asset = Core::AssetManager::GetAsset<Core::Asset>(arg);
                ImGui::BulletText("%s", _asset->GetName().c_str());

                _asset->OnGUIRender();
            }

            else if constexpr (std::is_same_v<T, std::monostate>)
            {
                ImGui::Text("Nothing Selected");
            }

    }, selected_object);

    ImGui::End();
#pragma endregion

    // TODO : Implement click select
#pragma region Scene
    ImGui::Begin("Scene", (bool*)0);

#pragma region Toolbar
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.4f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));

    ImGui::BeginChild("##SceneToolbar",
        ImVec2(220, 40),
        false,
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse 
    );

    if (ImGui::Button("T"))
        imguizmo_operation = ImGuizmo::TRANSLATE;

    ImGui::SameLine();

    if (ImGui::Button("R"))
        imguizmo_operation = ImGuizmo::ROTATE;

    ImGui::SameLine();

    if (ImGui::Button("S"))
        imguizmo_operation = ImGuizmo::SCALE;

    ImGui::SameLine();

    if (ImGui::Button("Local"))
        imguizmo_mode = ImGuizmo::LOCAL;

    ImGui::SameLine();

    if (ImGui::Button("World"))
        imguizmo_mode = ImGuizmo::WORLD;

    ImGui::EndChild();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
#pragma endregion

    Renderer::Renderer* _current_renderer = Renderer::Renderer::Get();
    Renderer::Framebuffer* _framebuffer = &_current_renderer->m_frame_buffer;

    if (_current_renderer->m_cameras.size() == 0 || _current_renderer->m_camera_index >= _current_renderer->m_cameras.size())
        ImGui::Text("INFO : No camera is rendering");

    ImVec2 viewport_size = ImGui::GetContentRegionAvail();

    if (viewport_size.x > 0 && viewport_size.y > 0)
    {
        if ((uint32_t)viewport_size.x != _framebuffer->width || (uint32_t)viewport_size.y != _framebuffer->height) // TODO : Remove this and store width and height in the renderer
        {
            //TODO : Once width and height are in the renderer, save it here too
            _framebuffer->Resize((uint32_t)viewport_size.x, (uint32_t)viewport_size.y);
            for (auto& camera : _current_renderer->m_cameras)
            {
                camera->SetAspect((float)viewport_size.x / (float)viewport_size.y);
            }
        }
        ImGui::Image(
            (void*)(intptr_t)_framebuffer->GetColorAttachmentRendererID(),
            viewport_size,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImVec2 image_min = ImGui::GetItemRectMin();
        ImVec2 image_max = ImGui::GetItemRectMax();
        ImVec2 image_size = ImVec2(image_max.x - image_min.x, image_max.y - image_min.y);

#pragma region StatsOverlay

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddRectFilled(image_min, ImVec2(image_min.x + 150, image_min.y + 60), IM_COL32(255, 255, 255, 40), 5.0f);

        float readable_fps = Time::readable_fps;

        std::ostringstream oss;

        oss << std::fixed << std::setprecision(1) << readable_fps;

        draw_list->AddText(ImVec2(image_min.x + 10, image_min.y + 10), IM_COL32(255, 255, 255, 255), std::string("FPS: " + oss.str()).c_str());
        draw_list->AddText(ImVec2(image_min.x + 10, image_min.y + 30), IM_COL32(255, 255, 255, 255), std::string("glErrorCode: " + std::to_string(_framebuffer->gl_error)).c_str());

#pragma endregion

#pragma region Guizmo
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

        ImGuizmo::SetRect(
            image_min.x,
            image_min.y,
            image_size.x,
            image_size.y
        );

        std::vector<Core::Camera*> cameras = Core::Application::Get()->m_renderer.get()->m_cameras;
        int camera_index = Core::Application::Get()->m_renderer.get()->m_camera_index;

        if (Core::Entity** selected_entity = std::get_if<Core::Entity*>(&selected_object))
        {
            if ((cameras.size() != 0 && *selected_entity != cameras[camera_index]->GetOwner()))
            {
                if (camera_index >= cameras.size())
                {
                    Core::LogMessageInfo("Invalid Camera Index. Switching camera_index to 0.");
                }

                Core::Transform* camera_transform = cameras[camera_index]->GetOwner()->GetComponent<Core::Transform>();
                glm::mat4* projection = cameras[camera_index]->GetPerspective();

                Core::Transform* transform =
                    (*selected_entity)->GetComponent<Core::Transform>();

                glm::mat4 world = transform->GetWorldTransformMatrix();

                glm::mat4 view =
                    glm::inverse(camera_transform->GetWorldTransformMatrix());

                ImGuizmo::Manipulate(
                    glm::value_ptr(view),
                    glm::value_ptr(*projection),
                    imguizmo_operation,
                    imguizmo_mode,
                    glm::value_ptr(world)
                );

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, rotation, scale;

                    ImGuizmo::DecomposeMatrixToComponents(
                        glm::value_ptr(world),
                        glm::value_ptr(translation),
                        glm::value_ptr(rotation),
                        glm::value_ptr(scale)
                    );

                    glm::vec3 radians = glm::radians(rotation);
                    glm::quat quat = glm::quat(radians);

                    transform->SetPosition(translation);
                    transform->SetRotation(quat);
                    transform->SetScale(scale);
                }
            }
        }
#pragma endregion
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

                Core::AssetManager::ImportAsset(path.c_str(), _state._current_folder);
                Core::LogMessage("Asset imported : " + path);
            }

            // TODO : Return false if import failed/pass to another layer

            return state;
        }
    );

    // TODO : FIX THIS !!!!! We should check if the input is keyboard or mouse before handling it ! Here it also intercepts events such as file drops, etc...
    // Actually, it intercepts all events marked as Core::event_category_input
    // We should also modify the condition as we want input inside the scene window (holding the framebuffer)
    if (_event.IsInCategory(Core::event_category_input))
    {
        if (ImGuizmo::IsUsing())
        {
            _event.handled = true;
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse)
        {
            _event.handled = true;  // Block input if ImGui is capturing it
            // TODO : Fix that and make the lock icon on the inspector
            //if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
                //selected_entity = nullptr;

            //TODO : Fix Input
            /*
            if (ImGui::IsAnyItemFocused())
                return;

            if (ImGui::IsKeyPressed(ImGuiKey_T))
            {
                imguizmo_operation = ImGuizmo::TRANSLATE;
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_R))
            {
                imguizmo_operation = ImGuizmo::ROTATE;
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_S))
            {
                imguizmo_operation = ImGuizmo::SCALE;
            }
            */

            return;
        }
    }
}

void EngineLayer::EngineLayer::LogMessage(std::string _message, LOG_PRIORITY _priority)
{
    std::tuple<LOG_PRIORITY, std::string> msg = std::make_tuple(_priority, _message);
    // TODO : Time + priority format
    m_message_pool.push_back(msg);
}

void EngineLayer::EngineLayer::ClearConsole()
{
    m_message_pool.clear();
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
        selected_object = _entity;
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

template <typename T>
std::string GetTypeName(T& obj)
{
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res{abi::__cxa_demangle(typeid(obj).name(), nullptr, nullptr, &status), std::free};
    return (status == 0) ? res.get() : typeid(obj).name();
}