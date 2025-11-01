#include "Graphics/DX/D3D11Device.h"
#include "InputLayout.h"
#include "Common.h"

using namespace NG_Engine;
using namespace Common;

void InputLayout::Create(DXDevice dx, const D3D11_INPUT_ELEMENT_DESC* desc, size_t count, ID3DBlob* blob)
{
    m_device = dx.device;
    m_context = dx.context;
    CHECK(m_device && m_context);
    CHECK(desc && blob);

    HRESULT hr = m_device->CreateInputLayout(
        desc,
        static_cast<UINT>(count),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        m_inputLayout.GetAddressOf());
    CHECK(SUCCEEDED(hr));
}

void InputLayout::SetIA(DXDevice dx)
{
    ID3D11DeviceContext* context = dx.context ? dx.context : m_context;
    CHECK(context);

    context->IASetInputLayout(m_inputLayout.Get());
}

void InputLayout::Destroy()
{
    m_inputLayout.Reset();
    m_device = nullptr;
    m_context = nullptr;
}
