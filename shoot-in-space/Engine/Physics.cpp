#include "Physics.h"
#include "Transform.h"
#include "RigidBody.h"

void Physics::Update(float dt, std::list<Entity> entities)
{
	for (Entity entity : entities)
	{
		RigidBody* rb = entity.GetComponent<RigidBody>();
		Transform* transform = entity.GetComponent<Transform>();
		transform->Position += rb->velocity * dt;
	}
}