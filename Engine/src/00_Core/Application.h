#pragma once
#include <string>
#include "Platform.h"
#include "Time.h"

// Graphics
#include "01_Graphics/DX/D3D11Device.h"
#include "01_Graphics/DX/SwapChain.h"
#include "01_Graphics/DX/RenderTarget.h"

namespace NG_Engine
{
    struct AppDesc
    {
        WinDesc window;
        bool         vsync{ false };
    };

    class Application : public IWinEvents
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        bool Initialize(const AppDesc& desc);
        int  Run();
        void Shutdown();

        // IWinEvents
        void OnResize(UINT width, UINT height) override;
        bool OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) override;

    protected: // hooks
        virtual void OnInit() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnRender() {}
        virtual void OnShutdown() {}

    protected:
        void BeginFrame(const float clear[4]);
        void EndFrame();

    protected:
        AppDesc      g_desc;
        Platform     g_platform;
        Time         g_time;

        D3D11Device  g_device;
        SwapChain    g_swapchain;
        RenderTarget g_backbuffer;

        bool         bRunning = false;
    };
} // namespace NG_Engine
