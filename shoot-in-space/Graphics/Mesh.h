#pragma once
#include "Manager.h"
#include "../Utils/Utils.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh
{
public:
    void Initialize(float aspectRatio, ID3D12Device* device);
    void Render(ID3D12GraphicsCommandList* commandList);
    
private:
    // App resources.
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    
};
