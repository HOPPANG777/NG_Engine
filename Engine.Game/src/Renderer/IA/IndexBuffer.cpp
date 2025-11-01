#include "Graphics/DX/D3D11Device.h"
#include "IndexBuffer.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void IndexBuffer::Create(DXDevice dx, const std::vector<UINT>& indices, const D3D11_USAGE& usage)
{
    m_device = dx.device;
    m_context = dx.context;
    CHECK(m_device && m_context);

    m_stride = sizeof(UINT);
    m_count = static_cast<UINT>(indices.size());
    m_offset = 0;

    D3D11_BUFFER_DESC desc{};
    desc.Usage = usage;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.ByteWidth = m_stride * m_count;

    switch (usage)
    {
    case D3D11_USAGE_DEFAULT:
    case D3D11_USAGE_IMMUTABLE:
        break;
    case D3D11_USAGE_DYNAMIC:
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    case D3D11_USAGE_STAGING:
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        break;
    }

    D3D11_SUBRESOURCE_DATA subData{};
    subData.pSysMem = indices.data();

    HRESULT hr = m_device->CreateBuffer(&desc, &subData, m_buffer.GetAddressOf());
    CHECK(SUCCEEDED(hr));
}

void IndexBuffer::SetIA(DXDevice dx)
{
    ID3D11DeviceContext* context = dx.context ? dx.context : m_context;
    CHECK(context);

    context->IASetIndexBuffer(
        m_buffer.Get(),
        DXGI_FORMAT_R32_UINT,
        0);
}

void IndexBuffer::Destroy()
{
    m_buffer.Reset();
    m_device = nullptr;
    m_context = nullptr;
    m_stride = 0;
    m_offset = 0;
    m_count = 0;
}
