#include "level_manager.h"

static Core::LevelManager* instance = nullptr;

Core::LevelManager::LevelManager(Level* _default_level) : m_current_level(_default_level)
{
	if (instance) // TODO : delete this + error
		return;

	instance = this;

	if (!m_current_level)
		m_current_level = new Level("Default Level");
}

Core::LevelManager::~LevelManager()
{
	if (m_current_level)
		delete m_current_level; // TODO : Same here, create/call an OnDestroy method
}

void Core::LevelManager::SwitchLevel(Level* _level)
{
	if (instance->m_current_level)
		delete instance->m_current_level; // TODO : Same here, create/call an OnDestroy method

	instance->m_current_level = _level;

	instance->m_current_level->OnAwake();

	instance->m_current_level->OnStart();
}

void Core::LevelManager::OnUpdate(float dt)
{
	instance->m_current_level->OnUpdate(dt);
}

void Core::LevelManager::OnRender()
{
	instance->m_current_level->OnRender();
}

void Core::LevelManager::OnGUIRender()
{
	instance->m_current_level->OnGUIRender();
}

Core::Level* Core::LevelManager::GetCurrentLevel()
{
	return instance->m_current_level;
}
