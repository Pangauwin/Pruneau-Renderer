#include "camera.h"

#include <glm/ext/matrix_clip_space.hpp>

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
}

void Core::Camera::SetCameraConfig(CameraConfig& _camera_config)
{
    m_config = _camera_config;
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
