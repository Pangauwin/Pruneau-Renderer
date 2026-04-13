#include "editor_camera.h"

#include "core/application.h"
#include "core/component.h"
#include "core/components/transform.h"
#include "core/entity.h"
#include "core/input/input.h"
#include "glm/common.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"
#include <string>

namespace Editor
{

EditorCamera::EditorCamera(Core::Entity* owner) : Core::Component(owner) 
{

}

EditorCamera::~EditorCamera()
{
    
}

void EditorCamera::OnUpdate(float dt)
{
    if(!Core::Application::Get()->m_engine_layer->is_renderer_focused)
        return;

    if(Core::Input::GetMouseButtonState(1) == Core::MOUSE_BUTTON_STATE_PRESSED &&
     (Core::Input::GetMouseDelta()[0] != 0 || Core::Input::GetMouseDelta()[1] != 0))
    {
        double delta_x = Core::Input::GetMouseDelta()[0];
        double delta_y = Core::Input::GetMouseDelta()[1];

        m_yaw -= delta_x * camera_sensitivity;
        m_pitch -= delta_y * camera_sensitivity;

        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

        glm::quat q_yaw = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0, 1, 0));
        glm::quat q_pitch = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1, 0, 0));

        glm::quat new_rotation = q_yaw * q_pitch;

        GetOwner()->GetComponent<Core::Transform>()->SetRotation(new_rotation);
    }

    bool has_moved = false;
    glm::vec3 movement_direction = glm::vec3(0, 0, 0);

    if(Core::Input::GetKeyState('W') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction += GetOwner()->GetComponent<Core::Transform>()->GetForward();
    }

    if(Core::Input::GetKeyState('S') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction -= GetOwner()->GetComponent<Core::Transform>()->GetForward();
    }

    if(Core::Input::GetKeyState('D') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction += GetOwner()->GetComponent<Core::Transform>()->GetRight();
    }

    if(Core::Input::GetKeyState('A') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction -= GetOwner()->GetComponent<Core::Transform>()->GetRight();
    }

    if(Core::Input::GetKeyState('E') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction += GetOwner()->GetComponent<Core::Transform>()->GetUp();
    }

    if(Core::Input::GetKeyState('Q') == Core::KEY_STATE_PRESSED)
    {
        has_moved = true;
        movement_direction -= GetOwner()->GetComponent<Core::Transform>()->GetUp();
    }

    if(!has_moved)
        return;
    
    if(glm::length(movement_direction) > 0)
        movement_direction = glm::normalize(movement_direction);
    movement_direction *= camera_speed * dt;

    GetOwner()->GetComponent<Core::Transform>()->Translate(movement_direction);
}

void EditorCamera::OnEditorRender()
{
    ImGui::DragFloat("Camera speed", &camera_speed);
    ImGui::DragFloat("Camera Sensitivity", &camera_sensitivity);
}

}