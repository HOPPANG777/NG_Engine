#include "DX/D3D11GraphicsEngine.h"

#include "Common.h"

using namespace NG_Engine;
using namespace Graphics;

bool D3D11GraphicsEngine::Initialize(const GraphicsInitDesc& desc)
{
    m_desc = desc;

    if (!m_device.Create(desc.width, desc.height, desc.adapterIndex)) return false;

    DXDevice dx = m_device.GetDXDevice();

    SwapChainDesc scd{};
    scd.hwnd = desc.window;
    scd.width = desc.width;
    scd.height = desc.height;
    scd.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.vsync = desc.vsync;
    scd.refreshNumerator = m_device.GetSelectedRefreshNumerator();
    scd.refreshDenominator = m_device.GetSelectedRefreshDenominator();

    if (!m_swapChain.Create(dx, scd))
    {
        m_device.Destroy();
        return false;
    }

    if (!m_backBuffer.CreateFromSwapChain(dx, m_swapChain))
    {
        m_swapChain.Destroy();
        m_device.Destroy();
        return false;
    }

    m_initialized = true;
    m_desc.adapterIndex = m_device.GetSelectedAdapterIndex();
    return true;
}

void D3D11GraphicsEngine::Shutdown()
{
    if (!m_initialized) return;

    m_backBuffer.Destroy();
    m_swapChain.Destroy();
    m_device.Destroy();

    m_initialized = false;
}

void D3D11GraphicsEngine::BeginFrame(const float clearColor[4])
{
    if (!m_initialized) return;

    m_backBuffer.Bind();
    m_backBuffer.Clear(clearColor);
}

void D3D11GraphicsEngine::EndFrame(bool vsync)
{
    if (!m_initialized) return;

    m_swapChain.Present(vsync ? 1u : 0u);
    m_desc.vsync = vsync;
}

void D3D11GraphicsEngine::Resize(UINT width, UINT height)
{
    if (!m_initialized) return;
    if (width == 0 || height == 0) return;

    DXDevice dx = m_device.GetDXDevice();

    m_backBuffer.Destroy();
    if (!m_swapChain.Resize(width, height)) return;
    if (!m_backBuffer.CreateFromSwapChain(dx, m_swapChain))
    {
        m_swapChain.Destroy();
        m_device.Destroy();
        m_initialized = false;
        return;
    }

    m_desc.width = width;
    m_desc.height = height;
}