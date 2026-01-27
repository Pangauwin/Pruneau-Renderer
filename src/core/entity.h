#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <memory>

#include "component.h"
#include "components/transform.h"

namespace Core {

class Entity
{
public:
	Entity(std::string _name, Entity* _parent = nullptr) : name(_name), parent(_parent) 
	{
		if (parent)
			parent->AddChild(this);
		AddComponent<Transform>();
	}

	~Entity()
	{
		for (auto& _child : children)
		{
			delete _child;
		}
	}

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	Entity(Entity&&) = delete;
	Entity& operator=(Entity&&) = delete;

	template <class T, class... Args>
	inline T& AddComponent(Args&& ...args)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component Class");
		auto& ptr = components[typeid(T)];

		if (!ptr) {
			ptr = std::make_unique<T>(this, std::forward<Args>(args)...); // TODO : Fix here
			ptr->OnAttach();
		}
		return static_cast<T&>(*ptr);
	}

	template <class T>
	T* GetComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component Class");
		auto it = components.find(typeid(T));
		if (it != components.end())
			return static_cast<T*>(it->second.get());
		return nullptr;
	}

	std::vector<Entity*> GetChildren()
	{
		return children;
	}

	void OnAwake();
	void OnStart();
	void OnUpdate(float dt);
	void OnRender();
	void OnGUIRender();

private:
	Entity* AddChild(Entity* _child)
	{
		children.push_back(_child);
		return this;
	}

	void RemoveChild(Entity* _child)
	{
		children.erase(find(children.begin(), children.end(), _child));
	}

public:
	std::string name;


	// TODO : Implement Getters Instead
public:
	std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
	std::vector<Entity*> children;
	Entity* parent;
};

}