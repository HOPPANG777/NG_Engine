#include "Graphics/DX/D3D11Device.h"
#include "ConstantBuffer.h"
#include "Common.h"

#include <cstring>

using namespace NG_Engine;
using namespace Common;

ConstantBuffer::ConstantBuffer(void* data, UINT dataSize)
    : m_data(data), m_dataSize(dataSize)
{
    m_desc.Usage = D3D11_USAGE_DYNAMIC;
    m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_desc.ByteWidth = dataSize;
    m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
}

void ConstantBuffer::Destroy()
{
    m_buffer.Reset();
    m_device = nullptr;
    m_context = nullptr;
    m_desc = {};
}

void ConstantBuffer::SetVSBuffer(DXDevice dx, UINT slot)
{
    EnsureResource(dx);
    MapData(m_data, m_dataSize);
    m_context->VSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
}

void ConstantBuffer::SetPSBuffer(DXDevice dx, UINT slot)
{
    EnsureResource(dx);
    MapData(m_data, m_dataSize);
    m_context->PSSetConstantBuffers(slot, 1, m_buffer.GetAddressOf());
}

void ConstantBuffer::EnsureResource(DXDevice dx)
{
    m_device = dx.device;
    m_context = dx.context;
    CHECK(m_device && m_context);

    if (!m_buffer)
    {
        HRESULT hr = m_device->CreateBuffer(&m_desc, nullptr, m_buffer.GetAddressOf());
        CHECK(SUCCEEDED(hr));
    }
}

void ConstantBuffer::MapData(const void* data, UINT dataSize)
{
    CHECK(m_context && m_buffer);

    D3D11_MAPPED_SUBRESOURCE mappedSubResource{};
    HRESULT hr = m_context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
    CHECK(SUCCEEDED(hr));

    std::memcpy(mappedSubResource.pData, data, dataSize);
    m_context->Unmap(m_buffer.Get(), 0);
}
