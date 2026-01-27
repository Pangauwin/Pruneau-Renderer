#include "level.h"

Core::Level::Level(const char* _name) : name(_name)
{

}

Core::Level::~Level()
{
	for (auto& e : entities)
	{
		delete e; // TODO : Do that a better way (implement OnDestroy or OnDelete or something like that
	}
}

Core::Entity* Core::Level::CreateEntity(const char* _name, Entity* _parent)
{
	entities.push_back(new Entity(_name, _parent));
	return entities.back();
}

void Core::Level::OnAwake()
{
	for (auto& e : entities)
	{
		e->OnAwake();
	}
}

void Core::Level::OnStart()
{
	for (auto& e : entities)
	{
		e->OnStart();
	}
}

void Core::Level::OnUpdate(float dt)
{
	for (auto& e : entities)
	{
		e->OnUpdate(dt);
	}
}

void Core::Level::OnRender()
{
	for (auto& e : entities)
	{
		e->OnRender();
	}
}

void Core::Level::OnGUIRender()
{
	for (auto& e : entities)
	{
		e->OnGUIRender();
	}
}
