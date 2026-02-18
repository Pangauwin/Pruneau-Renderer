#include "camera.h"

#include "renderer/renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

namespace Core {

    static bool registered = []()
        {
            AutoRegisterComponent<Camera>::Register("Camera");
            return true;
        }();

}

Core::Camera::Camera(Entity* owner) : Component(owner), m_config(CAMERA_TYPE_PERSPECTIVE, new CameraPerspectiveData()), m_projection(1.0f), m_updated(true) // Used by editor
{
    CameraPerspectiveData* data = static_cast<CameraPerspectiveData*>(m_config.data);

    m_projection = glm::perspective(data->fov, data->aspect, data->near_plane, data->far_plane);

    // Defensive: only register if renderer exists
    if (Renderer::Renderer::Get())
        Renderer::Renderer::Get()->RegisterCamera(this);
}

Core::Camera::Camera(Entity* _owner, CameraConfig& _camera_config) : Component(_owner), m_config(_camera_config), m_projection(glm::mat4(1.0f)), m_updated(true)
{
    if (m_config.type == CAMERA_TYPE_ORTHOGRAPHIC)
    {
        CameraOrthographicData* data = static_cast<CameraOrthographicData*>(m_config.data);

        m_projection = glm::ortho(data->left, data->right, data->bottom, data->top);
    }

    else
    {
        CameraPerspectiveData* data = static_cast<CameraPerspectiveData*>(m_config.data);

        m_projection = glm::perspective(data->fov, data->aspect, data->near_plane, data->far_plane);
    }

    if (Renderer::Renderer::Get())
        Renderer::Renderer::Get()->RegisterCamera(this);
}

Core::Camera::~Camera()
{
    if (Renderer::Renderer::Get())
        Renderer::Renderer::Get()->UnRegisterCamera(this);
}

void Core::Camera::SetCameraConfig(CameraConfig& _camera_config)
{
    // Replace our stored config with a deep copy and mark projection dirty
    m_config = _camera_config; // CameraConfig has safe copy semantics now
    m_updated = false;
}

void Core::Camera::SetAspect(float _aspect)
{
    if (!m_config.type == CAMERA_TYPE_PERSPECTIVE) return;

    CameraPerspectiveData* data = static_cast<CameraPerspectiveData*>(m_config.data);

    data->aspect = _aspect;

    m_updated = false;
}

void Core::Camera::OnEditorRender()
{
    if (m_config.type == CAMERA_TYPE_PERSPECTIVE)
    {
        CameraPerspectiveData* data = static_cast<CameraPerspectiveData*>(m_config.data);

        ImGui::DragFloat("Field of view", &data->fov);
        ImGui::DragFloat("Far Plane", &data->far_plane);
        ImGui::DragFloat("Near Plane", &data->near_plane);

        // user changed values -> mark dirty so next GetPerspective recomputes
        m_updated = false;
    }
}

glm::mat4* Core::Camera::GetPerspective()
{
    if (m_updated)
        return &m_projection;

    if (m_config.type == CAMERA_TYPE_ORTHOGRAPHIC)
    {
        CameraOrthographicData* data = static_cast<CameraOrthographicData*>(m_config.data);

        m_projection = glm::ortho(data->left, data->right, data->bottom, data->top);
    }

    else // if(m_config.type == CAMERA_TYPE_PERSPECTIVE)
    {
        CameraPerspectiveData* data = static_cast<CameraPerspectiveData*>(m_config.data);

        m_projection = glm::perspective(data->fov, data->aspect, data->near_plane, data->far_plane);
    }

    m_updated = true;

    return &m_projection;
}
