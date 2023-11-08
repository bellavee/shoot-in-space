#pragma once
#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Engine/IComponent.h"
#include <memory>
using namespace DirectX;

class RenderItem : public IComponent
{
public:
	
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;
	void AddEntity(int entityid, std::shared_ptr<RenderItem> renderItem);
	int _entityId;
	std::unordered_map<int, std::shared_ptr<RenderItem>> _entitiesRenderItem;
	XMFLOAT4X4 World = MathHelper::Identity4x4();
	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	int NumFramesDirty = gNumFrameResources;
	UINT ObjCBIndex = -1;
	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

