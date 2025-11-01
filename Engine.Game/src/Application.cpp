#include "Application.h"
#include "IGraphicsEngine.h"

using namespace NG_Engine;

Application::Application(Graphics::IGraphicsEngine& graphics)
    : g_graphics(graphics)
{

}

bool Application::Initialize(const AppDesc& desc)
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

    Graphics::GraphicsInitDesc graphicsDesc{};
    graphicsDesc.window = g_platform.g_handle;
    graphicsDesc.width = g_platform.g_width;
    graphicsDesc.height = g_platform.g_height;
    graphicsDesc.vsync = desc.vsync;
    if (!g_graphics.Initialize(graphicsDesc))
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

        const float clear[4]{ 0.1f, 0.1f, 0.12f, 1.0f };
        BeginFrame(clear);
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
    return false; // 메세지가 필요할시
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
