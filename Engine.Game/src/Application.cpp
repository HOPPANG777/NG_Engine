#include "Application.h"
#include "IGraphicsEngine.h"
#include "Common.h"
#include "ConstantColor.h"

using namespace NG_Engine;
using namespace Common;
using namespace ConstantColor;

Application::Application(Graphics::IGraphicsEngine& graphics)
    : g_graphics(graphics)
{

}

bool Application::Init(const AppDesc& desc)
{
    g_desc = desc;

    WinDesc wd{};
    wd.appName = desc.window.appName;
    wd.instance = desc.window.instance;
    wd.width = desc.window.width;
    wd.height = desc.window.height;
    wd.icon = desc.window.icon;

    if (!g_platform.CreateWindowClass(wd)) return false;
    if (!g_platform.CreateWindowInstance(wd, this)) return false;
    g_platform.Show();

    g_desc.window.handle = g_platform.g_handle;
    g_desc.window.width = g_platform.g_width;
    g_desc.window.height = g_platform.g_height;

    g_mouse.Init(g_platform.g_handle);

    Graphics::GraphicsInitDesc graphicsDesc{};
    graphicsDesc.window = g_platform.g_handle;
    graphicsDesc.width = g_platform.g_width;
    graphicsDesc.height = g_platform.g_height;
    graphicsDesc.vsync = desc.vsync;
    graphicsDesc.adapterIndex = desc.adapterIndex;
    if (!g_graphics.Init(graphicsDesc))
    {
        g_platform.Destroy();
        return false;
    }

    bGraphicsInitialized = true;

    g_time.Reset();
    OnInit();
    bRunning = true;
    return true;
}

int Application::Run()
{
    while (bRunning)
    {
        if (!g_platform.PumpOnce())
        {
            bRunning = false;
            break;
        }

        float dt = g_time.Tick();

        OnUpdate(dt);

        BeginFrame(Gray);
        OnRender();
        EndFrame();
    }

    Shutdown();
    return 0;
}

void Application::Shutdown()
{
    bRunning = false;

    OnShutdown();

    if (bGraphicsInitialized)
    {
        g_graphics.Shutdown();
        bGraphicsInitialized = false;
    }

    g_platform.Destroy();
}

void Application::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;
    if (!bGraphicsInitialized) return;

    g_desc.window.width = width;
    g_desc.window.height = height;

    g_graphics.Resize(width, height);
}

bool Application::OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool handled = false;
    handled = g_mouse.ProcessMessage(message, wParam, lParam) || handled;

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_UNICHAR:
        handled = g_keyboard.ProcessMessage(message, wParam, lParam) || handled;
        break;
    default:
        break;
    }

    return handled;
}

void Application::BeginFrame(const float clear[4])
{
    if (!bGraphicsInitialized) return;

    g_graphics.BeginFrame(clear);
}

void Application::EndFrame()
{
    if (!bGraphicsInitialized) return;

    g_graphics.EndFrame(g_desc.vsync);
}

void Application::UpdateInputDevices()
{
    g_keyboard.Update();
    g_mouse.Update();
}