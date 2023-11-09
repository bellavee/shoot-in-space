#include "RigidBody.h"

RigidBody::RigidBody()
{
	Velocity = XMFLOAT3(0, 0, 0);
}
RigidBody::~RigidBody() {}
void RigidBody::AddEntity(int entityid, std::shared_ptr<RigidBody> rigidBody)
{
	_entityId = entityid;
	_entitiesRB[_entityId] = rigidBody;
}