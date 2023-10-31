#pragma once

#include "Manager.h"
#include "Mesh.h"

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
    UINT64 m_fenceValue;
    
    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();

    void CreateRootSignature();
    void CreateShadersAndPSO();
    void CreateCommandList();
    MeshData* CreateMesh();
    void CreateDescriptorHeaps();
    void CreateFrameResources();
    void CreateSyncObjects();
    void CreateConstantBuffer();

    void RenderMesh(const MeshData* mesh);
    void RenderAllMeshes();

    MeshData* newMesh;
    MeshData* newMesh1;
    MeshData* newMesh2;

};