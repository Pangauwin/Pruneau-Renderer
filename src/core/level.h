#pragma once

#include <string>

#include "entity.h"

#include "components/camera.h"
#include "components/model_renderer.h"

namespace Core
{
class Level
{

public:
	Level(const char* _name);
	~Level();

    // Forbids copy and affectation
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    // Allows move if needed
    Level(Level&&) = default;
    Level& operator=(Level&&) = default;

    Entity* CreateEntity(const char* _name, Entity* _parent);

    void OnAwake();
    void OnStart();
    void OnUpdate(float dt);
    void OnRender();
    void OnGUIRender();

    std::string name;
    std::vector<Entity*> entities;
};
}