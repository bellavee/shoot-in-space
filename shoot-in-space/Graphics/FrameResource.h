#pragma once

#include "../Utils/MathHelper.h"
#include "../Utils/pch.h"
#include "../Utils/UploadBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};
    
struct SceneConstantBuffer
{
    XMMATRIX TransformationMatrix;
    XMFLOAT4 Offset;
    float Padding[60];
};

struct MeshData {
    ComPtr<ID3D12Resource> VertexBuffer;
    ComPtr<ID3D12Resource> IndexBuffer;
    D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
    D3D12_INDEX_BUFFER_VIEW IndexBufferView;
    ComPtr<ID3D12Resource> ConstantBuffer;
    SceneConstantBuffer ConstantBufferData;
    void* PCbvDataBegin = nullptr;
    bool MoveDirection;
    XMFLOAT3 Direction;
};

struct ObjectConstants
{
    XMFLOAT4X4 World = MathHelper::Identity4x4();
};

struct PassConstants
{
    XMFLOAT4X4 View = MathHelper::Identity4x4();
    XMFLOAT4X4 InvView = MathHelper::Identity4x4();
    XMFLOAT4X4 Proj = MathHelper::Identity4x4();
    XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
    XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
    XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
    XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
    float CbPerObjectPad1 = 0.0f;
    XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
    XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
};

struct FrameResource
{
    FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount);
    FrameResource(const FrameResource& rhs) = delete;
    FrameResource& operator = (const FrameResource& rhs) = delete;
    ~FrameResource();
    
    ComPtr<ID3D12CommandAllocator> CommandListAlloc;
    std::unique_ptr<UploadBuffer<PassConstants>> PassCb = nullptr;
    std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCb = nullptr;
    
    UINT64 Fence = 0;
};
