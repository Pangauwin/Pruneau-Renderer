#include "level.h"
#include "core/entity.h"
#include <vector>

Core::Level::Level(const char* _name) : name(_name)
{

}

Core::Level::~Level()
{
	for (auto& e : entities)
	{
		if(e->parent == nullptr)
		{
			DestroyEntity(e);
		}
	}
}

Core::Entity* Core::Level::CreateEntity(const char* _name, Entity* _parent)
{
	entities.push_back(new Entity(_name, _parent));
	return entities.back();
}

void Core::Level::DestroyEntity(Core::Entity* _entity)
{
	_entity->OnDestroy();

	for(Entity* _ent: _entity->GetChildren())
	{
		DestroyEntity(_ent);
	}

	std::vector<Entity*>* parent_vec = &_entity->parent->children;
	parent_vec->erase(find(parent_vec->begin(), parent_vec->end(), _entity));

	entities.erase(find(entities.begin(), entities.end(), _entity));
	delete _entity;
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
