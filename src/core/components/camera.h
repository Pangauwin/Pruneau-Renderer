#pragma once

#include "../component.h"

#include <glm/glm.hpp>

namespace Renderer
{
class Renderer;
}

namespace Core
{

enum CAMERA_TYPE
{
	CAMERA_TYPE_PERSPECTIVE,
	CAMERA_TYPE_ORTHOGRAPHIC
};

struct CameraData { 
	virtual ~CameraData() = default;
};

struct CameraPerspectiveData : public CameraData
{
	float fov = 70.f;
	float aspect = 16.f / 9.f;
	float near_plane = 0.1f;
	float far_plane = 100.f;
};

struct CameraOrthographicData : public CameraData
{
	float top = 50.f;
	float bottom = -50.f;
	float left = -50.f;
	float right = 50.f;
};

struct CameraConfig {
	CAMERA_TYPE type;
	CameraData* data;

	CameraConfig(CAMERA_TYPE _type, CameraData* _data) : type(_type), data(nullptr)
	{
		if (type == CAMERA_TYPE_PERSPECTIVE)
		{
			data = new CameraPerspectiveData();
		}

		else
		{
			data = new CameraOrthographicData();
		}

		*data = *_data;
	}

	~CameraConfig()
	{
		delete data;
	}
};

class Camera : public Component
{
friend class Renderer::Renderer;

public:
	Camera(Entity* _owner, CameraConfig& _camera_config);

	void SetCameraConfig(CameraConfig& _camera_config);

private:
	glm::mat4* GetPerspective();

private:
	glm::mat4 m_projection;
	CameraConfig m_config;
	bool m_updated;
};
}