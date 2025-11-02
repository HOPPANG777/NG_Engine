#pragma once
#include <string>
#include "Core/Platform.h"
#include "Core/Time.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace NG_Engine
{
    struct AppDesc
    {
        WinDesc window;
        bool vsync = false;
        int adapterIndex = -1;
    };

    namespace Graphics
    {
        class IGraphicsEngine;
    }

    class Application : public IWinEvents
    {
    public:
        explicit Application(Graphics::IGraphicsEngine& graphics);
        virtual ~Application() = default;

        bool Init(const AppDesc& desc);
        int  Run();
        void Shutdown();

        // IWinEvents
        void OnResize(UINT width, UINT height) override;
        bool OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) override;

    protected: // hooks
        virtual void OnInit() {}
        virtual void OnUpdate(float dt) = 0;
        virtual void OnRender() = 0;
        virtual void OnShutdown() {}

    protected:
        void UpdateInputDevices();
        Keyboard& GetKeyboard() { return g_keyboard; }
        const Keyboard& GetKeyboard() const { return g_keyboard; }
        Mouse& GetMouse() { return g_mouse; }
        const Mouse& GetMouse() const { return g_mouse; }

    protected:
        void BeginFrame(const float clear[4]);
        void EndFrame();

    protected:
        AppDesc      g_desc;
        Platform     g_platform;
        Time         g_time;
        Keyboard     g_keyboard;
        Mouse        g_mouse;

        Graphics::IGraphicsEngine& g_graphics;
        bool         bRunning = false;
        bool         bGraphicsInitialized = false;
    };
} // namespace NG_Engine
