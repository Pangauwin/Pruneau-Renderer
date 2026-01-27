#include "transform.h"

#include "../entity.h"
#include <algorithm>

#include <imgui.h>

Core::Transform::Transform(Entity* _owner, glm::vec3 _position, glm::quat _rotation, glm::vec3 _scale) : 
    Component(_owner), m_world_transform(glm::mat4(1.0f)), m_parent(nullptr), m_updated(true), 
    m_forward(glm::vec3()), m_position(_position), m_right(glm::vec3()), m_rotation(_rotation), m_scale(_scale), m_up(glm::vec3())
{
    m_local_transform = glm::translate(m_world_transform, _position) * glm::mat4_cast(_rotation) * glm::scale(glm::mat4(1.0f), _scale);

    if (_owner->parent)
    {
        m_parent = _owner->GetComponent<Transform>()->RegisterChild(this);
        m_parent->UpdateMatrix();
        m_world_transform = m_parent->GetWorldTransformMatrix() * m_local_transform;
    }

    else
    {
        m_world_transform = m_local_transform;
    }

}

Core::Transform::~Transform()
{
    m_parent->UnRegisterChild(this);
}

#pragma region MatrixGetters

glm::mat4 Core::Transform::GetWorldTransformMatrix()
{
    if (!m_updated)
        UpdateMatrix();
    return m_world_transform;
}

glm::mat4 Core::Transform::GetLocalTransformMatrix()
{
    if (!m_updated)
        UpdateMatrix();
    return m_local_transform;
}

#pragma endregion

#pragma region Operators

void Core::Transform::SetPosition(glm::vec3& _position)
{
    m_position = _position;
    m_updated = false;
}

void Core::Transform::SetRotation(glm::quat& _rotation)
{
    m_rotation = _rotation;
    m_updated = false;
}

void Core::Transform::SetScale(glm::vec3& _scale)
{
    m_scale = _scale;
    m_updated = false;
}

void Core::Transform::Translate(glm::vec3& _translation)
{
    m_position += _translation;
}

void Core::Transform::Rotate(glm::quat& _rotation)
{
    m_rotation *= _rotation;
    m_updated = false;
}

void Core::Transform::Scale(glm::vec3& _scale)
{
    m_scale = glm::vec3(m_scale.x * _scale.x, m_scale.y * _scale.y, m_scale.z * _scale.z);
    m_updated = false;
}

void Core::Transform::Scale(float _factor)
{
    m_scale *= _factor;
    m_updated = false;
}

#pragma endregion

#pragma region Getters

glm::vec3 Core::Transform::GetPosition() const
{
    return m_position;
}

glm::quat Core::Transform::GetRotation() const
{
    return m_rotation;
}

glm::vec3 Core::Transform::GetScale() const
{
    return m_scale;
}

glm::vec3 Core::Transform::GetForward() const
{
    return m_forward;
}

glm::vec3 Core::Transform::GetUp() const
{
    return m_up;
}

glm::vec3 Core::Transform::GetRight() const
{
    return m_right;
}

void Core::Transform::OnEditorRender()
{
    ImGui::DragFloat3("Position", &m_position.x, 0.1f);
    ImGui::DragFloat3("Scale", &m_scale.x, 0.1f);

    // TODO : Finish
}

#pragma endregion

void Core::Transform::UpdateMatrix()
{
    m_local_transform = glm::mat4(1.0f);
    m_local_transform = glm::translate(m_local_transform, m_position);
    m_local_transform *= glm::mat4_cast(m_rotation);
    m_local_transform = glm::scale(m_local_transform, m_scale);

    glm::mat3 m_mat3_local_transform = glm::mat3(m_local_transform);

    m_up = m_mat3_local_transform * glm::vec3(0.0f, 1.0f, 0.0f);
    m_forward = m_mat3_local_transform * glm::vec3(0.0f, 0.0f, -1.0f);
    m_right = m_mat3_local_transform * glm::vec3(1.0f, 0.0f, 0.0f);

    if (m_parent)
    {
        m_world_transform = m_parent->GetWorldTransformMatrix() * m_local_transform;
    }
    else
    {
        m_world_transform = m_local_transform;
    }

    for (auto& it : m_children)
    {
        it->UpdateMatrix();
    }
}

Core::Transform* Core::Transform::RegisterChild(Core::Transform* _child)
{
    m_children.push_back(_child);
    return this;
}

void Core::Transform::UnRegisterChild(Core::Transform* _child)
{
    m_children.erase(find(m_children.begin(), m_children.end(), _child));
}