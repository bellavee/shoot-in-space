#pragma once

#include "pch.h"
#include "Utils.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : 
        m_isConstantBuffer(isConstantBuffer)
    {
        m_elementByteSize = sizeof(T);

        if (isConstantBuffer) m_elementByteSize = AlignTo256(sizeof(T));
        
        CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize * elementCount);

        ComPtr<ID3D12Resource> buffer;
        ThrowIfFailed(device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_uploadBuffer)));
        
        ThrowIfFailed(m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedData)));
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator = (const UploadBuffer& rhs) = delete;

    ~UploadBuffer()
    {
        if (m_uploadBuffer != nullptr) m_uploadBuffer->Unmap(0, nullptr);
        m_mappedData = nullptr;
    }

    ID3D12Resource* Resource() const
    {
        return m_uploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&m_mappedData[elementIndex * m_elementByteSize], &data, sizeof(T));
    }
    

private:
    ComPtr<ID3D12Resource> m_uploadBuffer;
    BYTE* m_mappedData = nullptr;
    UINT m_elementByteSize = 0;
    bool m_isConstantBuffer = false;
    
};
