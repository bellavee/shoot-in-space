#pragma once
#include <DirectXMath.h>
#include "IComponent.h"
#include <unordered_map>
#include <memory>
using namespace DirectX;

class RigidBody : public IComponent
{
public:
	XMFLOAT3 Velocity;
	float Mass;

	void AddEntity(int entityid, std::shared_ptr<RigidBody> transform);
	int _entityId;
	std::unordered_map<int, std::shared_ptr<RigidBody>> _entitiesRB;
};
