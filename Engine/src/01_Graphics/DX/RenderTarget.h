#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace NG_Engine
{
    class D3D11Device;
    class SwapChain;

    class RenderTarget
    {
    public:
        RenderTarget() = default;
        ~RenderTarget() { Destroy(); }

        bool CreateFromSwapChain(const D3D11Device& device, const SwapChain& swapChain);
        void Destroy();

        void Bind(const D3D11Device& device);                    // OMSetRenderTargets + Viewport
        void Clear(const D3D11Device& device, const float color[4]);

        ID3D11RenderTargetView* GetRTV() const { return m_rtv.Get(); }
        const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

    private:
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv = nullptr;
        D3D11_VIEWPORT m_viewport{ 
            0.0f, 0.0f, 
            0.0f, 0.0f, 
            0.0f, 1.0f 
        };
    };
} // namespace NG_Engine
