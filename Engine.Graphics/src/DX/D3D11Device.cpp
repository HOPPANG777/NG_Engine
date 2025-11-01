#include "DX/D3D11Device.h"
#include "Common.h"   // CHECK

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Graphics;
using namespace Common;

bool D3D11Device::Create()
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL created{};

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        levels, _countof(levels),
        D3D11_SDK_VERSION,
        m_device.ReleaseAndGetAddressOf(),
        &created,
        m_context.ReleaseAndGetAddressOf()
    );
    CHECK(SUCCEEDED(hr) || m_device);   // 디바이스 생성 확인

    if (FAILED(hr))
    {
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            flags,
            levels, _countof(levels),
            D3D11_SDK_VERSION,
            m_device.ReleaseAndGetAddressOf(),
            &created,
            m_context.ReleaseAndGetAddressOf()
        );
        CHECK(SUCCEEDED(hr));            // WARP 폴백 확인
    }
    return SUCCEEDED(hr);
}

void D3D11Device::Destroy()
{
    if (m_context) m_context->ClearState();
    m_context.Reset();
    m_device.Reset();
}
