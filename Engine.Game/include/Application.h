#pragma once
#include <string>
#include "Core/Platform.h"
#include "Core/Time.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Managers/SceneManager/SceneManager.h"
#include "Debug/NEGUI.h"

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
        virtual void OnFixedUpdate(double fixedDt) {}
        virtual void OnUpdate(double dt) = 0;
        virtual void OnLateUpdate(double dt) {}
        virtual void OnRender() = 0;
        virtual void OnPostRender() {}
        virtual void OnGUI() {}
        virtual void OnShutdown() {}

    protected:
        void UpdateInputDevices();
        Keyboard& GetKeyboard() { return m_keyboard; }
        const Keyboard& GetKeyboard() const { return m_keyboard; }
        Mouse& GetMouse() { return m_mouse; }
        const Mouse& GetMouse() const { return m_mouse; }

    protected:
        SceneManager& GetSceneManager() { return g_sceneManager; }
        const SceneManager& GetSceneManager() const { return g_sceneManager; }
        void SetFixedTimeStep(double fixedDt);
        double GetFixedTimeStep() const { return g_fixedTimeStep; }

    protected:
        void BeginFrame(const float clear[4]);
        void EndFrame();

    protected:
        AppDesc      m_desc;
        Platform     m_platform;
        Time         m_time;
        Keyboard     m_keyboard;
        Mouse        m_mouse;
        NEGUI        m_gui;

        Graphics::IGraphicsEngine& m_graphics;
        bool         bRunning = false;
        bool         bGraphicsInitialized = false;
        SceneManager g_sceneManager;
        double        g_fixedTimeStep = 1.0f / 60.0f;
        double        g_fixedAccumulator = 0.0f;
    };
} // namespace NG_Engine
