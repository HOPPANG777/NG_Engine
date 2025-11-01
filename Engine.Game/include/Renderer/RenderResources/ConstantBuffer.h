#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "EngineAPI.h"

namespace NG_Engine
{
    class ConstantBuffer
    {
    public:
        void SetVSBuffer(DXDevice dx, UINT slot);
        void SetPSBuffer(DXDevice dx, UINT slot);

    protected:
        ConstantBuffer(void* data, UINT dataSize);
        ~ConstantBuffer()
        {
            Destroy();
        }

        void Destroy();

    private:
        void EnsureResource(DXDevice dx);
        void MapData(const void* data, UINT dataSize);

    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;

        D3D11_BUFFER_DESC m_desc{};
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

        void* m_data = nullptr;
        UINT m_dataSize = 0;
    };
}
