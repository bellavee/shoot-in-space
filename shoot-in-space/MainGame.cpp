#include "Engine/Transform.h"
#include "MainGame.h"
#include "Engine/RigidBody.h"

void MainGame::CreateEntities() {

}

void MainGame::Init(Game* game) {

	Entity* entity = new Entity();
	std::shared_ptr<Transform> tr = entity->AddComponent<Transform>();
	std::shared_ptr<RigidBody> rb = entity->AddComponent<RigidBody>();
	game->BuildSphereItem(0, tr->Position,rb->Velocity);
}

//void Update();