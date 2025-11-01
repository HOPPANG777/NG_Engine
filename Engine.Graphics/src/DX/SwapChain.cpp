#include "DX/D3D11Device.h"
#include "DX/SwapChain.h"
#include "Common.h"

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Graphics;
using namespace Common;

namespace
{
    constexpr UINT DEFAULT_REFRESH_NUMERATOR = 60;
    constexpr UINT DEFAULT_REFRESH_DENOMINATOR = 1;
}

static ComPtr<IDXGIFactory> CreateFactory()
{
    ComPtr<IDXGIFactory> f;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)f.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    return f;
}

bool SwapChain::Create(DXDevice dx, const SwapChainDesc& desc)
{
    m_device = dx.device;
    m_context = dx.context;
    CHECK(m_device && m_context);
    CHECK(desc.hwnd != nullptr);

    m_hwnd = desc.hwnd;
    m_format = desc.format;
    m_bufferCount = desc.bufferCount ? desc.bufferCount : 2;
    m_vsync = desc.vsync;
    m_refreshNumerator = desc.refreshNumerator;
    m_refreshDenominator = desc.refreshDenominator ? desc.refreshDenominator : 1;
    m_width = desc.width;
    m_height = desc.height;

    DXGI_SWAP_CHAIN_DESC scd{};
    scd.BufferDesc.Width = 0;
    scd.BufferDesc.Height = 0;
    if (m_vsync)
    {
        UINT numerator = m_refreshNumerator ? m_refreshNumerator : DEFAULT_REFRESH_NUMERATOR;
        UINT denominator = m_refreshDenominator ? m_refreshDenominator : DEFAULT_REFRESH_DENOMINATOR;
        scd.BufferDesc.RefreshRate.Numerator = numerator;
        scd.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 1;
    }
    scd.BufferDesc.Format = m_format;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = m_bufferCount;
    scd.OutputWindow = m_hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ComPtr<IDXGIDevice>  dxgiDevice;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory> factory;

    HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    hr = adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
    CHECK(SUCCEEDED(hr));

    hr = factory->CreateSwapChain(m_device, &scd, m_swapChain.ReleaseAndGetAddressOf());
    CHECK(SUCCEEDED(hr));

    return SUCCEEDED(hr);
}

void SwapChain::Destroy()
{
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(FALSE, nullptr);
    }
    m_swapChain.Reset();
    m_device = nullptr;
    m_context = nullptr;
    m_refreshNumerator = 0;
    m_refreshDenominator = 1;
}

bool SwapChain::Resize(UINT w, UINT h)
{
    CHECK(m_swapChain);
    if (w == 0 || h == 0) return true;

    m_width = w; m_height = h;
    HRESULT hr = m_swapChain->ResizeBuffers(0, w, h, m_format, 0);
    CHECK(SUCCEEDED(hr));
    return SUCCEEDED(hr);
}

void SwapChain::Present(UINT syncInterval)
{
    CHECK(m_swapChain);
    m_swapChain->Present(syncInterval, 0);
}
