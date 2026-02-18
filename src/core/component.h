#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <typeindex>
#include <memory>

namespace Core {

class Entity;

class Component {
public:
	Component(Entity* _owner) : m_owner(_owner) {}

	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;
	Component(const Component&&) = delete;
	Component&& operator=(const Component&&) = delete;

	virtual ~Component() = default;
	virtual void OnAwake() {}
	virtual void OnStart() {}
	virtual void OnAttach() {}
	virtual void OnUpdate(float dt) { (void)dt; }
	virtual void OnRender() {}
	virtual void OnEditorRender() {} //TODO : create a defined variable that make this function build on editor version only
	virtual void OnGUIRender() {}
	virtual Entity* GetOwner() { return m_owner; }

private:
	Entity* m_owner;
};

struct ComponentInfo
{
	std::string name;
	std::function<std::unique_ptr<Component>(Entity*)> factory;
};

struct ComponentRegistry
{
public:
	static void Register(std::type_index type, ComponentInfo info)
	{
		Get()[type] = std::move(info);
	}

	static const auto& All()
	{
		return Get();
	}

private:
	static std::unordered_map<std::type_index, ComponentInfo>& Get()
	{
		static std::unordered_map<std::type_index, ComponentInfo> map;
		return map;
	}
};

template<typename T>
struct AutoRegisterComponent
{
	static void Register(const std::string& name)
	{
		ComponentRegistry::Register(
			typeid(T),
			ComponentInfo{
				name,
				[](Entity* owner)
				{
					return std::make_unique<T>(owner);
				}
			}
		);
	}
};

}