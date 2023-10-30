﻿#pragma once

#include <stdexcept>

#include "pch.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

inline std::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
    HRESULT Error() const { return m_hr; }
private:
    const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}

struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

struct SceneConstantBuffer
{
    XMMATRIX transformationMatrix;
    XMFLOAT4 offset;
    float padding[60]; // Padding so the constant buffer is 256-byte aligned.
};

inline UINT AlignTo256(UINT size) {
    return (size + 255) & ~255;
}

template<typename T>
void CopyDataToBuffer(ComPtr<ID3D12Resource>& buffer, T* data, size_t dataSize)
{
    UINT8* pDataBegin;
    CD3DX12_RANGE readRange(0, 0); 
    ThrowIfFailed(buffer->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)));
    memcpy(pDataBegin, data, dataSize);
    buffer->Unmap(0, nullptr);
}

template <typename BufferType>
ComPtr<ID3D12Resource> CreateBuffer(ComPtr<ID3D12Device> device, UINT itemCount)
{
    const UINT bufferSize = sizeof(BufferType) * itemCount; 
    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    ComPtr<ID3D12Resource> buffer;
    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)));
    
    return buffer;
}

struct MeshData {
    ComPtr<ID3D12Resource> vertexBuffer;
    ComPtr<ID3D12Resource> indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW indexBufferView;
    ComPtr<ID3D12Resource> constantBuffer;
    SceneConstantBuffer constantBufferData;
    void* pCbvDataBegin = nullptr;
    
};