#pragma once

#include "../component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace Core
{
class Transform : public Component
{
public:
	Transform(Entity* _owner, glm::vec3 _position = glm::vec3(), glm::quat _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~Transform();

	glm::mat4 GetWorldTransformMatrix();
	glm::mat4 GetLocalTransformMatrix();

	void SetPosition(glm::vec3& _position);
	void SetRotation(glm::quat& _rotation);
	void SetScale(glm::vec3& _scale);

	void Translate(glm::vec3& _translation);
	void Rotate(glm::quat& _rotation);
	void Scale(glm::vec3& _scale);
	void Scale(float _factor);

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetScale() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void OnEditorRender();

private:
	void UpdateMatrix();
	Transform* RegisterChild(Transform* _child);
	void UnRegisterChild(Transform* _child);

private:
	bool m_updated;
	glm::mat4 m_world_transform;
	glm::mat4 m_local_transform;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	Transform* m_parent;

	std::vector<Transform*> m_children;
};
}