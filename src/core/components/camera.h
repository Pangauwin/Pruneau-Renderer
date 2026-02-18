#pragma once

#include "../component.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

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
	virtual CameraData* Clone() const = 0;
};

struct CameraPerspectiveData : public CameraData
{
	float fov = 70.f;
	float aspect = 16.f / 9.f;
	float near_plane = 0.1f;
	float far_plane = 100.f;

	CameraPerspectiveData* Clone() const override { return new CameraPerspectiveData(*this); }
};

struct CameraOrthographicData : public CameraData
{
	float top = 50.f;
	float bottom = -50.f;
	float left = -50.f;
	float right = 50.f;

	CameraOrthographicData* Clone() const override { return new CameraOrthographicData(*this); }
};

struct CameraConfig {
	CAMERA_TYPE type;
	CameraData* data;

	CameraConfig(CAMERA_TYPE _type, CameraData* _data) : type(_type), data(nullptr)
	{
		if (_data)
		{
			data = _data->Clone();
		}
		else
		{
			if (type == CAMERA_TYPE_PERSPECTIVE)
				data = new CameraPerspectiveData();
			else
				data = new CameraOrthographicData();
		}
	}

	// copy ctor
	CameraConfig(const CameraConfig& other) : type(other.type), data(nullptr)
	{
		if (other.data) data = other.data->Clone();
	}

	// copy assign
	CameraConfig& operator=(const CameraConfig& other)
	{
		if (this == &other) return *this;
		type = other.type;
		delete data;
		data = other.data ? other.data->Clone() : nullptr;
		return *this;
	}

	~CameraConfig()
	{
		delete data;
	}
};

class Camera : public Component, AutoRegisterComponent<Camera>
{
friend class Renderer::Renderer;

public:
	explicit Camera(Entity* owner); // Used by editor

	Camera(Entity* _owner, CameraConfig& _camera_config);
	~Camera();

	void SetCameraConfig(CameraConfig& _camera_config);
	void SetAspect(float _aspect);

	void OnEditorRender();
	glm::mat4* GetPerspective();

private:
	glm::mat4 m_projection;
	CameraConfig m_config;
	bool m_updated;
};
}