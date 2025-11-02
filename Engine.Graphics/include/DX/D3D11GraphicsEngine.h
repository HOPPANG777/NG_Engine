#pragma once

#include "IGraphicsEngine.h"
#include "D3D11Device.h"
#include "SwapChain.h"
#include "RenderTarget.h"

namespace NG_Engine
{
    namespace Graphics
    {

        class D3D11GraphicsEngine final : public IGraphicsEngine
        {
        public:
            D3D11GraphicsEngine() = default;
            ~D3D11GraphicsEngine() override = default;

            bool Init(const GraphicsInitDesc& desc) override;
            void Shutdown() override;

            void BeginFrame(const float clearColor[4]) override;
            void EndFrame(bool vsync) override;

            void Resize(UINT width, UINT height) override;

            DXDevice GetDXDevice() const override { return m_device.GetDXDevice(); }


        private:
            GraphicsInitDesc m_desc{};
            D3D11Device m_device;
            SwapChain m_swapChain;
            RenderTarget m_backBuffer;
            bool m_initialized = false;

        };
    }
}