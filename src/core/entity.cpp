#include "entity.h"

void Core::Entity::OnAwake()
{
	for (auto& _comp : components)
	{
		_comp.second->OnAwake();
	}

	for (auto& _child : children)
	{
		_child->OnAwake();
	}
}

void Core::Entity::OnStart()
{
	for (auto& _comp : components)
	{
		_comp.second->OnStart();
	}

	for (auto& _child : children)
	{
		_child->OnStart();
	}
}

void Core::Entity::OnUpdate(float dt)
{
	for (auto& _comp : components)
	{
		_comp.second->OnUpdate(dt);
	}

	for (auto& _child : children)
	{
		_child->OnUpdate(dt);
	}
}

void Core::Entity::OnRender()
{
	for (auto& _comp : components)
	{
		_comp.second->OnRender();
	}

	for (auto& _child : children)
	{
		_child->OnRender();
	}
}

void Core::Entity::OnGUIRender()
{
	for (auto& _comp : components)
	{
		_comp.second->OnGUIRender();
	}

	for (auto& _child : children)
	{
		_child->OnGUIRender();
	}
}
