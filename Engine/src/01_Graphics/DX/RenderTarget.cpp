#include "D3D11Device.h"
#include "SwapChain.h"
#include "RenderTarget.h"
#include "Common.h"

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Common;

bool RenderTarget::CreateFromSwapChain(const D3D11Device& device, const SwapChain& swap)
{
    CHECK(device.GetDevice() && swap.Get());

    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swap.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    CHECK(SUCCEEDED(hr));

    hr = device.GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.ReleaseAndGetAddressOf());
    CHECK(SUCCEEDED(hr));

    DXGI_SWAP_CHAIN_DESC scd{};
    swap.Get()->GetDesc(&scd);

    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    m_viewport.Width = static_cast<FLOAT>(scd.BufferDesc.Width);
    m_viewport.Height = static_cast<FLOAT>(scd.BufferDesc.Height);

    if (m_viewport.Width == 0.0f || m_viewport.Height == 0.0f)
    {
        RECT rc{};
        GetClientRect(scd.OutputWindow, &rc);
        m_viewport.Width = FLOAT(rc.right - rc.left);
        m_viewport.Height = FLOAT(rc.bottom - rc.top);
    }

    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    return true;
}

void RenderTarget::Destroy()
{
    m_rtv.Reset();
}

void RenderTarget::Bind(const D3D11Device& device)
{
    CHECK(device.GetDC() && m_rtv);
    auto* ctx = device.GetDC();
    ctx->RSSetViewports(1, &m_viewport);
    ID3D11RenderTargetView* rtv = m_rtv.Get();
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
}

void RenderTarget::Clear(const D3D11Device& device, const float color[4])
{
    CHECK(device.GetContext());
    if (m_rtv)
        device.GetDC()->ClearRenderTargetView(m_rtv.Get(), color);
}
