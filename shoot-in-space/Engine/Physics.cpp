#include "Physics.h"
#include "Transform.h"
#include "RigidBody.h"

void Physics::Update(float dt, std::list<Entity> entities)
{
	for (Entity entity : entities)
	{
		RigidBody* rb = entity.GetComponent<RigidBody>();
		Transform* transform = entity.GetComponent<Transform>();
		transform->Position.x += rb->Velocity.x * dt;
		transform->Position.y += rb->Velocity.y * dt;
		transform->Position.z += rb->Velocity.z * dt;
	}
}