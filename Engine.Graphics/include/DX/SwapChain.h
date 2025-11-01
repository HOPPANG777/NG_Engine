#pragma once

#include "d3d11.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <cstdint>

namespace NG_Engine
{
    namespace Graphics
    {
        struct SwapChainDesc
        {
            HWND        hwnd = nullptr;
            UINT        width = 0;
            UINT        height = 0;
            DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
            bool        vsync = false;
            UINT        bufferCount = 2;
            UINT        refreshNumerator = 0;
            UINT        refreshDenominator = 1;
        };

        class D3D11Device; // fwd
        struct DXDevice;

        class SwapChain
        {
        public:
            SwapChain() = default;
            ~SwapChain() { Destroy(); }

            bool Create(Graphics::DXDevice device, const SwapChainDesc& desc);
            void Destroy();

            bool Resize(UINT width, UINT height);             // 0
            void Present(UINT syncInterval);                  // vsync: 1/0

        public:
            IDXGISwapChain* Get() const { return m_swapChain.Get(); }

        private:
            ID3D11Device* m_device = nullptr;           //
            ID3D11DeviceContext* m_context = nullptr;   //

            Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain = nullptr;

            //
            HWND        m_hwnd = nullptr;
            DXGI_FORMAT m_format = DXGI_FORMAT_B8G8R8A8_UNORM;
            UINT        m_bufferCount = 2;
            bool        m_vsync = false;
            UINT        m_refreshNumerator = 0;
            UINT        m_refreshDenominator = 1;

            UINT        m_width = 0;
            UINT        m_height = 0;
        };
    }

} // namespace NG_Engine
