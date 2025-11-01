#include "DX/D3D11Device.h"
#include "Common.h"   // CHECK

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using Microsoft::WRL::ComPtr;
using namespace NG_Engine;
using namespace Graphics;
using namespace Common;

namespace
{
    constexpr UINT DEFAULT_REFRESH_NUMERATOR = 60;
    constexpr UINT DEFAULT_REFRESH_DENOMINATOR = 1;
}

bool D3D11Device::Create(UINT width, UINT height, int preferredAdapter)
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

    EnumerateAdapters(width, height);

    IDXGIAdapter1* adapter = nullptr;
    if (!m_adapterInfos.empty())
    {
        m_selectedAdapterIndex = ChooseAdapterIndex(preferredAdapter);
        if (m_selectedAdapterIndex >= 0 && m_selectedAdapterIndex < static_cast<int>(m_adapterInfos.size()))
        {
            const AdapterInfo& selected = m_adapterInfos[static_cast<size_t>(m_selectedAdapterIndex)];
            adapter = selected.adapter.Get();
            m_selectedRefreshNumerator = selected.output.refreshNumerator;
            m_selectedRefreshDenominator = selected.output.refreshDenominator ?
                selected.output.refreshDenominator : DEFAULT_REFRESH_DENOMINATOR;
        }
    }

    if (adapter == nullptr)
    {
        m_selectedAdapterIndex = -1;
        m_selectedRefreshNumerator = DEFAULT_REFRESH_NUMERATOR;
        m_selectedRefreshDenominator = DEFAULT_REFRESH_DENOMINATOR;
    }

    HRESULT hr = D3D11CreateDevice(
        adapter,
        adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        levels, _countof(levels),
        D3D11_SDK_VERSION,
        m_device.ReleaseAndGetAddressOf(),
        &created,
        m_context.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr) && adapter)
    {
        // Adapter-specific creation failed, try default hardware adapter.
        hr = D3D11CreateDevice(
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
        if (SUCCEEDED(hr))
        {
            m_selectedAdapterIndex = -1;
            m_selectedRefreshNumerator = DEFAULT_REFRESH_NUMERATOR;
            m_selectedRefreshDenominator = DEFAULT_REFRESH_DENOMINATOR;
        }
    }

    CHECK(SUCCEEDED(hr) || m_device);   // ̽  Ȯ

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
        CHECK(SUCCEEDED(hr));            // WARP  Ȯ
        if (SUCCEEDED(hr))
        {
            m_selectedAdapterIndex = -1;
            m_selectedRefreshNumerator = DEFAULT_REFRESH_NUMERATOR;
            m_selectedRefreshDenominator = DEFAULT_REFRESH_DENOMINATOR;
        }
    }

    if (SUCCEEDED(hr))
    {
        PrintAdapterSummary();
    }

    return SUCCEEDED(hr);
}

void D3D11Device::Destroy()
{
    if (m_context) m_context->ClearState();
    m_context.Reset();
    m_device.Reset();

    m_adapterInfos.clear();
    m_selectedAdapterIndex = -1;
    m_selectedRefreshNumerator = DEFAULT_REFRESH_NUMERATOR;
    m_selectedRefreshDenominator = DEFAULT_REFRESH_DENOMINATOR;
}

void D3D11Device::EnumerateAdapters(UINT width, UINT height)
{
    m_adapterInfos.clear();
    m_selectedAdapterIndex = -1;

    ComPtr<IDXGIFactory1> factory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf()));
    if (FAILED(hr))
    {
        CHECK(SUCCEEDED(hr));
        return;
    }

    UINT index = 0;
    while (true)
    {
        ComPtr<IDXGIAdapter1> adapter;
        hr = factory->EnumAdapters1(index, adapter.GetAddressOf());
        if (hr == DXGI_ERROR_NOT_FOUND)
        {
            break;
        }
        CHECK(SUCCEEDED(hr));

        AdapterInfo info{};
        info.ordinal = index;
        info.adapter = adapter;
        adapter->GetDesc1(&info.desc);
        EnumerateAdapterOutput(info, width, height);

        m_adapterInfos.emplace_back(std::move(info));
        ++index;
    }
}

void D3D11Device::EnumerateAdapterOutput(AdapterInfo& info, UINT width, UINT height)
{
    info.output.refreshNumerator = DEFAULT_REFRESH_NUMERATOR;
    info.output.refreshDenominator = DEFAULT_REFRESH_DENOMINATOR;

    ComPtr<IDXGIOutput> output;
    HRESULT hr = info.adapter->EnumOutputs(0, output.GetAddressOf());
    if (hr == DXGI_ERROR_NOT_FOUND)
    {
        return;
    }
    CHECK(SUCCEEDED(hr));

    info.output.output = output;
    output->GetDesc(&info.output.desc);

    UINT numModes = 0;
    const DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    if (FAILED(hr) || numModes == 0)
    {
        return;
    }

    std::vector<DXGI_MODE_DESC> modes(numModes);
    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, modes.data());
    if (FAILED(hr))
    {
        return;
    }

    auto selectBestRefresh = [&](bool matchResolution) -> bool
    {
        UINT bestNum = 0;
        UINT bestDen = 1;

        for (const DXGI_MODE_DESC& mode : modes)
        {
            if (matchResolution && (mode.Width != width || mode.Height != height))
            {
                continue;
            }

            UINT num = mode.RefreshRate.Numerator;
            UINT den = mode.RefreshRate.Denominator ? mode.RefreshRate.Denominator : DEFAULT_REFRESH_DENOMINATOR;

            if (static_cast<uint64_t>(num) * bestDen > static_cast<uint64_t>(bestNum) * den)
            {
                bestNum = num;
                bestDen = den;
            }
        }

        if (bestNum > 0)
        {
            info.output.refreshNumerator = bestNum;
            info.output.refreshDenominator = bestDen;
            return true;
        }
        return false;
    };

    bool matchedResolution = (width != 0 && height != 0);
    if (!selectBestRefresh(matchedResolution))
    {
        selectBestRefresh(false);
    }
}

int D3D11Device::ChooseAdapterIndex(int preferred) const
{
    if (m_adapterInfos.empty())
    {
        return -1;
    }

    if (preferred >= 0 && preferred < static_cast<int>(m_adapterInfos.size()))
    {
        return preferred;
    }

    UINT64 bestMemory = 0;
    int bestIndex = 0;
    for (size_t i = 0; i < m_adapterInfos.size(); ++i)
    {
        UINT64 mem = m_adapterInfos[i].desc.DedicatedVideoMemory;
        if (mem > bestMemory)
        {
            bestMemory = mem;
            bestIndex = static_cast<int>(i);
        }
    }
    return bestIndex;
}

void D3D11Device::PrintAdapterSummary() const
{
    if (m_adapterInfos.empty())
    {
        std::wcout << L"No DXGI adapters found. Using default configuration." << std::endl;
        return;
    }

    for (const AdapterInfo& info : m_adapterInfos)
    {
        const UINT64 vramMB = info.desc.DedicatedVideoMemory / (1024ull * 1024ull);
        std::wcout << L"[Adapter " << info.ordinal << L"] " << info.desc.Description << std::endl;
        std::wcout << L"    VRAM : " << vramMB << L" MB" << std::endl;

        if (info.output.output)
        {
            float refresh = static_cast<float>(info.output.refreshNumerator) /
                static_cast<float>(info.output.refreshDenominator ? info.output.refreshDenominator : DEFAULT_REFRESH_DENOMINATOR);
            std::wcout << L"    Output : " << info.output.desc.DeviceName
                << L" (" << refresh << L" Hz)" << std::endl;
        }
        else
        {
            std::wcout << L"    Output : None" << std::endl;
        }

        std::wcout << std::endl;
    }

    if (m_selectedAdapterIndex >= 0 && m_selectedAdapterIndex < static_cast<int>(m_adapterInfos.size()))
    {
        std::wcout << L"Selected Adapter : [" << m_selectedAdapterIndex << L"] "
            << m_adapterInfos[static_cast<size_t>(m_selectedAdapterIndex)].desc.Description << std::endl;
    }
    else
    {
        std::wcout << L"Selected Adapter : Default hardware" << std::endl;
    }
}
