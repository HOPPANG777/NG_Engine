#include "D3D11Device.h"
#include "SwapChain.h"
#include "Common.h"

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Common;

static ComPtr<IDXGIFactory> CreateFactory()
{
    ComPtr<IDXGIFactory> f;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)f.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    return f;
}

bool SwapChain::Create(const D3D11Device& device, const SwapChainDesc& desc)
{
    CHECK(device.GetDevice() && device.GetDC());
    CHECK(desc.hwnd != nullptr);

    m_hwnd = desc.hwnd;
    m_format = desc.format;
    m_bufferCount = desc.bufferCount ? desc.bufferCount : 2;
    m_vsync = desc.vsync;
    m_width = desc.width;
    m_height = desc.height;

    DXGI_SWAP_CHAIN_DESC scd{};
    scd.BufferDesc.Width = 0;
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.RefreshRate.Numerator = m_vsync ? 60 : 0;
    scd.BufferDesc.RefreshRate.Denominator = 1;
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

    HRESULT hr = device.GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
    CHECK(SUCCEEDED(hr));
    hr = adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
    CHECK(SUCCEEDED(hr));

    hr = factory->CreateSwapChain(device.GetDevice(), &scd, m_swapChain.ReleaseAndGetAddressOf());
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
