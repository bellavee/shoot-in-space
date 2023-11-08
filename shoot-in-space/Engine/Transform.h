#pragma once

#include <DirectXMath.h>
#include <unordered_map>
#include <memory>
#include "IComponent.h"

using namespace DirectX;

class Transform : public IComponent
{
public:
	XMFLOAT3 Scale;
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	void AddEntity(int entityId, std::shared_ptr<Transform> transform);
	void SetPostion(int x, int y, int z);
	void UpdateMatrix();
	void Rotate(float yaw, float pitch, float roll);
	void RotateYaw(float yaw);
	void RotatePitch(float pitch);
	void RotateRoll(float roll);
	std::unordered_map<int, std::shared_ptr<Transform>> GetComponents();
private:
	float m_roll, m_pitch, m_yaw = 0;
	float m_xScale, m_yScale, m_zScale = 1;

	XMVECTOR m_vDir;
	XMVECTOR m_vRight;
	XMVECTOR m_vUp;

	XMMATRIX m_mRot;
	XMMATRIX m_matrix;
	int _entityId;
	std::unordered_map<int, std::shared_ptr<Transform>> _entitiesTransform;
};
