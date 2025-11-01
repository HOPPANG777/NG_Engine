#pragma once

#include "Common.h"

namespace NG_Engine
{
    template <typename T>
    inline void VertexBuffer::Create(DXDevice dx, const std::vector<T>& vertices, const D3D11_USAGE& usage)
    {
        m_device = dx.device;
        m_context = dx.context;
        Common::CHECK(m_device && m_context);

        m_stride = static_cast<UINT>(sizeof(T));
        m_count = static_cast<UINT>(vertices.size());
        m_offset = 0;

        D3D11_BUFFER_DESC desc{};
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
        subData.pSysMem = vertices.data();

        HRESULT hr = m_device->CreateBuffer(&desc, &subData, m_buffer.GetAddressOf());
        Common::CHECK(SUCCEEDED(hr));
    }
}
