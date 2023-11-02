
#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT meshVertexCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CommandListAlloc.GetAddressOf())));

    PassCb = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
    ObjectCb = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
    MeshVb = std::make_unique<UploadBuffer<Vertex>>(device, meshVertexCount, false);
}

FrameResource::~FrameResource()
{

}