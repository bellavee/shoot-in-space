#pragma once
#include <vector>
#include "Engine/Entity.h"
#include "Graphics/Game.h"

class MainGame
{

public:
	std::vector<Entity> entites;
	static void CreateEntities();

	static void Init(Game* game);

};

