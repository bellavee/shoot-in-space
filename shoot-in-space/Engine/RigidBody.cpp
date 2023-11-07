#include "RigidBody.h"

void RigidBody::AddEntity(int entityid, std::shared_ptr<RigidBody> rigidBody)
{
	_entityId = entityid;
	_entitiesRB[_entityId] = rigidBody;
}