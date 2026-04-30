#pragma once

#include "../component.h"
#include "core/entity.h" 

namespace Editor
{
class EditorCamera : public Core::Component
{
public:
    EditorCamera(Core::Entity* owner);
    ~EditorCamera();

    void OnUpdate(float dt);
    void OnEditorRender();

private:
    float camera_speed = 4.0f;
    float camera_sensitivity = 0.15f;
    // TODO: finish

    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
};
}