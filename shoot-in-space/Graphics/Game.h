#pragma once

#include "Camera.h"
#include "Manager.h"
#include "FrameResource.h"
#include "../Utils/MathHelper.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Game : public Manager
{
public:
    Game(UINT width, UINT height, std::wstring name);
    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

private:
    
    std::vector<MeshData*> m_meshes;
    
    static const UINT FrameCount = 2;
    
    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_rtvDescriptorSize;
    
    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValues[FrameCount];

    // Frame resources
    std::vector<std::unique_ptr<FrameResource>> m_frameResources;
    FrameResource* m_currFrameResource = nullptr;
    int m_currFrameResourceIndex = 0;
    PassConstants m_mainPassCb;

    XMFLOAT3 m_eyePos = { 0.0f, 0.0f, 0.0f };
    XMFLOAT4X4 m_view = MathHelper::Identity4x4();
    XMFLOAT4X4 m_proj = MathHelper::Identity4x4();

    float m_theta = 1.5f*XM_PI;
    float m_phi = XM_PIDIV2 - 0.1f;
    float m_radius = 50.0f;

    float m_sunTheta = 1.25f*XM_PI;
    float m_sunPhi = XM_PIDIV4;

    POINT m_lastMousePos;
    
    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void MoveToNextFrame();
    void WaitForGpu();

    void CreateRootSignature();
    void CreateShadersAndPSO();
    void CreateCommandList();
    void CreateMesh(XMFLOAT3 positionOffset);
    void CreateMeshes(int n);
    void CreateSyncObjects();
    void CreateConstantBuffer() const;

    void RenderMesh(const MeshData* mesh) const;
    void RenderAllMeshes() const;

    void CreateFrameResources();
    void UpdateCamera();
    void UpdateObjectCbs();
    void UpdateMainPassCb();
    
    Camera m_camera;

};