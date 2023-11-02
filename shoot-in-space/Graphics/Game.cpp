
#include "../Utils/pch.h"
#include "../Utils/Utils.h"
#include "Game.h"
#include "../Window/Window.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

Game::Game(UINT width, UINT height, std::wstring name) :
    Manager(width, height, name),
    m_frameIndex(0),
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
    m_rtvDescriptorSize(0)
{
}

void Game::OnInit()
{
    LoadPipeline();
    LoadAssets();
}

void Game::OnUpdate()
{
    const float translationSpeed = 0.003f;
    const float offsetBounds = 0.5f;

    static float direction = 1.0f; 

    RenderAllMeshes();

    newMesh->constantBufferData.offset.x += translationSpeed * direction;
    if (abs(newMesh->constantBufferData.offset.x) > offsetBounds) {
        direction = -direction;  
        newMesh->constantBufferData.offset.x += translationSpeed * direction;  
    }

    // newMesh1->constantBufferData.offset.x += translationSpeed * direction;
    // if (abs(newMesh1->constantBufferData.offset.x) > offsetBounds) {
    //     direction = -direction;  
    //     newMesh1->constantBufferData.offset.x += translationSpeed * direction;  
    // }
    //
    newMesh1->constantBufferData.offset.y += translationSpeed * direction;
    if (abs(newMesh1->constantBufferData.offset.y) > offsetBounds) {
        direction = -direction;  
        newMesh1->constantBufferData.offset.y += translationSpeed * direction;  
    }
    
    CopyMemory(newMesh->pCbvDataBegin, &newMesh->constantBufferData, sizeof(newMesh->constantBufferData));
    CopyMemory(newMesh1->pCbvDataBegin, &newMesh1->constantBufferData, sizeof(newMesh1->constantBufferData));
}


void Game::OnRender()
{
    PopulateCommandList();
    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    ThrowIfFailed(m_swapChain->Present(1, 0));
    WaitForPreviousFrame();
}

void Game::OnDestroy()
{
    WaitForPreviousFrame();
    CloseHandle(m_fenceEvent);
}

// ---------- Initialize DirectX ----------

void Game::LoadAssets()
{
    CreateRootSignature();
    CreateShadersAndPSO();
    CreateCommandList();
    newMesh = CreateMesh();
    newMesh1 = CreateMesh();
    newMesh2 = CreateMesh();
    CreateConstantBuffer();
    CreateSyncObjects();
}

void Game::LoadPipeline()
{
    UINT dxgiFactoryFlags = 0;
    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    if (m_useWarpDevice)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)
            ));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(factory.Get(), &hardwareAdapter);

        ThrowIfFailed(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)
            ));
    }

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),
        Window::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
        ));

    // This sample does not support fullscreen transitions.
    ThrowIfFailed(factory->MakeWindowAssociation(Window::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain.As(&m_swapChain));
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    CreateDescriptorHeaps();
    CreateFrameResources();

    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

void Game::PopulateCommandList()
{
    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
    m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // m_commandList->SetGraphicsRootConstantBufferView(1, mesh->constantBuffer->GetGPUVirtualAddress());
    // m_commandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);
    
    const CD3DX12_RESOURCE_BARRIER rtvBackBuffer = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &rtvBackBuffer);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Render
    const float clearColor[] = { 0.6f, 0.8f, 1.0f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    RenderAllMeshes();
    
    const CD3DX12_RESOURCE_BARRIER presentBackBuffer = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &presentBackBuffer);

    ThrowIfFailed(m_commandList->Close());
}

void Game::WaitForPreviousFrame()
{
    const UINT64 fence = m_fenceValue;
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
    m_fenceValue++;

    if (m_fence->GetCompletedValue() < fence)
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void Game::CreateRootSignature()
{
    CD3DX12_ROOT_PARAMETER1 rootParameters[2];

    // ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
    // rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
    rootParameters[0].InitAsConstantBufferView(0);
    rootParameters[1].InitAsConstantBufferView(1);

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
    ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void Game::CreateShadersAndPSO()
{
     ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"Graphics\\Shaders\\shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"Graphics\\Shaders\\shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

void Game::CreateCommandList()
{
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
    ThrowIfFailed(m_commandList->Close());
}

void Game::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    ThrowIfFailed(m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));
}

void Game::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT n = 0; n < FrameCount; n++)
    {
        ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
        m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }
}

void Game::CreateSyncObjects()
{
    ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValue = 1;
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
    WaitForPreviousFrame();
}

MeshData* Game::CreateMesh()
{
    MeshData* mesh = new MeshData();    
    Vertex vertices[] =
    {
        { { -0.1f, -0.1f * m_aspectRatio, 0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } },  // Top-left vertex Front
        { { -0.1f, 0.1f * m_aspectRatio, 0.1f }, { 1.0f, 1.0f, 0.0f, 1.0f } },   // Top-right vertex Front
        { { 0.1f, 0.1f * m_aspectRatio, 0.1f }, { 0.0f, 1.0f, 1.0f, 1.0f } },  // Bottom-right vertex Front
        { { 0.1f, -0.1f * m_aspectRatio, 0.1f }, { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bottom-left vertex Front

        { { 0.1f, -0.1f * m_aspectRatio, -0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f } },  // Top-left vertex Back
        { { 0.1f, 0.1f * m_aspectRatio, -0.1f }, { 1.0f, 1.0f, 0.0f, 1.0f } },   // Top-right vertex Back
        { { -0.1f, 0.1f * m_aspectRatio, -0.1f }, { 0.0f, 1.0f, 1.0f, 1.0f } },  // Bottom-right vertex Back
        { { -0.1f, -0.1f * m_aspectRatio, -0.1f }, { 0.0f, 0.0f, 1.0f, 1.0f } }  // Bottom-left vertex Back
    };

    UINT indices[] =
    {
        0, 1, 3,
        1, 2, 3,   // Front
        3, 2, 4,
        2, 5, 4,   // Right
        4, 5, 7,
        5, 6, 7,   // Back
        7, 6, 0,
        6, 1, 0,   // Left
        1, 6, 2,
        6, 5, 2,   // Top
        7, 0, 4,
        0, 3, 4,   // Bottom
    };

    m_indicesCount = sizeof(indices) / 4;
        
    mesh->indexBuffer = CreateBuffer<UINT>(m_device, _countof(indices));
    CopyDataToBuffer(mesh->indexBuffer, indices, sizeof(indices));
    mesh->indexBufferView.BufferLocation = mesh->indexBuffer->GetGPUVirtualAddress();
    mesh->indexBufferView.SizeInBytes = sizeof(indices);
    mesh->indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    mesh->vertexBuffer = CreateBuffer<Vertex>(m_device, _countof(vertices));
    CopyDataToBuffer(mesh->vertexBuffer, vertices, sizeof(vertices));
    mesh->vertexBufferView.BufferLocation = mesh->vertexBuffer->GetGPUVirtualAddress();
    mesh->vertexBufferView.StrideInBytes = sizeof(Vertex);
    mesh->vertexBufferView.SizeInBytes = sizeof(vertices);


    m_meshes.push_back(mesh);
    return m_meshes.back();
}

void Game::CreateConstantBuffer()
{
    for (MeshData* mesh : m_meshes)
    {
        mesh->constantBuffer = CreateBuffer<SceneConstantBuffer>(m_device, 1);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = mesh->constantBuffer->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = AlignTo256(sizeof(SceneConstantBuffer));
        m_device->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetCPUDescriptorHandleForHeapStart());

        CD3DX12_RANGE readRange(0, 0);
        ThrowIfFailed(mesh->constantBuffer->Map(0, &readRange, &mesh->pCbvDataBegin));
        memcpy(mesh->pCbvDataBegin, &mesh->constantBufferData, sizeof(mesh->constantBufferData));
    }
}

void Game::RenderMesh(const MeshData* mesh)
{
    m_commandList->SetGraphicsRootConstantBufferView(0, mesh->constantBuffer->GetGPUVirtualAddress());
    m_commandList->IASetVertexBuffers(0, 1, &mesh->vertexBufferView);
    m_commandList->IASetIndexBuffer(&mesh->indexBufferView);
    m_commandList->DrawIndexedInstanced(m_indicesCount, 1, 0, 0, 0);
}

void Game::RenderAllMeshes()
{
    for (const MeshData* mesh : m_meshes)
    {
        RenderMesh(mesh);
    }
}