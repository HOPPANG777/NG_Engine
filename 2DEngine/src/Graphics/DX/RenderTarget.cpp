#include "D3D11Device.h"
#include "SwapChain.h"
#include "RenderTarget.h"
#include "Common.h"

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Common;

bool RenderTarget::CreateFromSwapChain(DXDevice dx, const SwapChain& swap)
{
    m_device = dx.device;
    m_context = dx.context;
    CHECK(m_device && m_context && swap.Get());

    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swap.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    CHECK(SUCCEEDED(hr));

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.ReleaseAndGetAddressOf());
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
    m_device = nullptr;
    m_context = nullptr;
}

void RenderTarget::Bind(DXDevice dx)
{
    CHECK(m_context && m_rtv);
    auto* ctx = m_context;
    ctx->RSSetViewports(1, &m_viewport);
    ID3D11RenderTargetView* rtv = m_rtv.Get();
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
}

void RenderTarget::Clear(DXDevice dx, const float color[4])
{
    CHECK(m_context);
    if (m_rtv)
    {
        m_context->ClearRenderTargetView(m_rtv.Get(), color);
    }
}
