#pragma once

#include "level.h"

namespace Core
{
class LevelManager
{
public:
	LevelManager(Level* _default_level = nullptr);
	~LevelManager();

	static void SwitchLevel(Level* _level);

	static void OnUpdate(float dt);
	static void OnRender();
	static void OnGUIRender();

	static Level* GetCurrentLevel();

private:
	Level* m_current_level;
};
}