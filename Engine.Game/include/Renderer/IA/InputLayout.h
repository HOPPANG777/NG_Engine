#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "EngineAPI.h"

namespace NG_Engine
{
    class InputLayout
    {
    public:
        ~InputLayout()
        {
            Destroy();
        }

        void Create(DXDevice dx, const D3D11_INPUT_ELEMENT_DESC* desc, size_t count, ID3DBlob* blob);
        void SetIA(DXDevice dx);

        void Destroy();

    public:
        ID3D11InputLayout* GetResource() const { return m_inputLayout.Get(); }

    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;

        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    };
} // namespace NG_Engine
