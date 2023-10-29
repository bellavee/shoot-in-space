#pragma once

#include "../Utils/Utils.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh
{
public:
    void Initialize(Vertex* vertices, UINT vertexCount, ID3D12Device* device);
    void Render(ID3D12GraphicsCommandList* commandList);
    
private:
    // App resources.
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    UINT m_vertexCount;

    
};
