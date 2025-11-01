#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "EngineAPI.h"

namespace NG_Engine
{
    class IndexBuffer
    {
    public:
        ~IndexBuffer()
        {
            Destroy();
        }

        void Create(DXDevice dx, const std::vector<UINT>& indices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);
        void SetIA(DXDevice dx);

        void Destroy();

    public:
        ID3D11Buffer* GetResource() const { return m_buffer.Get(); }
        UINT GetStride() const { return m_stride; }
        UINT GetOffset() const { return m_offset; }
        UINT GetCount() const { return m_count; }

    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;

        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        UINT m_stride = 0;
        UINT m_offset = 0;
        UINT m_count = 0;
    };
} // namespace NG_Engine
