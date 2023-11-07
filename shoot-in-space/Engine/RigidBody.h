#pragma once
#include <DirectXMath.h>
#include "IComponent.h"
#include <unordered_map>
#include <memory>
using namespace DirectX;

struct RigidBody : public IComponent
{
	XMVECTOR velocity;
	float mass;
	void AddEntity(int entityid, std::shared_ptr<RigidBody> transform);
	int _entityId;
	std::unordered_map<int, std::shared_ptr<RigidBody>> _entitiesRB;
};
