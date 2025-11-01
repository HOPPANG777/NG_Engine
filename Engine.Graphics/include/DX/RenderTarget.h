#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace NG_Engine
{
    namespace Graphics
    {
        class D3D11Device;
        class SwapChain;

        class RenderTarget
        {
        public:
            RenderTarget() = default;
            ~RenderTarget() { Destroy(); }

            bool CreateFromSwapChain(DXDevice dx, const SwapChain& swapChain);
            void Destroy();

            void Bind(DXDevice dx);                    // OMSetRenderTargets + Viewport
            void Clear(DXDevice dx, const float color[4]);

            ID3D11RenderTargetView* GetRTV() const { return m_rtv.Get(); }
            const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

        private:
            ID3D11Device* m_device = nullptr;           // 비소유
            ID3D11DeviceContext* m_context = nullptr;   // 비소유

            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv = nullptr;
            D3D11_VIEWPORT m_viewport{
                0.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 1.0f
            };
        };
    }

} // namespace NG_Engine
