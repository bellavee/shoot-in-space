#include "EntityManager.h"
#include "Transform.h"

Entity EntityManager::CreatEntity(std::string name)
{
	Entity entity = Entity(name);
	entity.AddComponent<Transform>();
	return entity;
}