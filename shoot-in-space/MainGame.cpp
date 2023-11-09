
#include "Engine/Transform.h"

void MainGame::CreateEntities() {

}

void MainGame::Init(Game* game) {

	Entity* entity = new Entity("test");
	entity->AddComponent<Transform>();
	std::shared_ptr<Transform> tr = entity->AddComponent<Transform>();
	game->BuildSphereItem(0, tr->Position);

	game->BuildSphereItem(0, {1, 1, 1});
	game->BuildSphereItem(0, {1, 2, 1});
	game->BuildSphereItem(0, {1, 1, 2});
	game->BuildSphereItem(0, {3, 1, 1});
	game->BuildSphereItem(0, {1, 2, 1});
}

void Update();