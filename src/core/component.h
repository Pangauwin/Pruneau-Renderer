#pragma once

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

}