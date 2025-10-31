//#include "Include.h"
#include "Application.h"

using namespace NG_Engine;

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

    if (!g_device.Create()) return false;

    SwapChainDesc scd{};
    scd.hwnd = g_platform.g_handle;
    scd.width = g_platform.g_width;
    scd.height = g_platform.g_height;
    scd.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.vsync = desc.vsync;
    if (!g_swapchain.Create(g_device, scd)) return false;

    if (!g_backbuffer.CreateFromSwapChain(g_device, g_swapchain)) return false;

    g_time.Reset();
    OnInit();
    bRunning = true;
    return true;
}

int Application::Run()
{
    while (bRunning)
    {
        if (!g_platform.PumpOnce()) break;

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
    OnShutdown();
    g_backbuffer.Destroy();
    g_swapchain.Destroy();
    g_device.Destroy();
    g_platform.Destroy();
}

void Application::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;

    g_backbuffer.Destroy();
    g_swapchain.Resize(width, height);
    g_backbuffer.CreateFromSwapChain(g_device, g_swapchain);
}

bool Application::OnMsg(HWND, UINT, WPARAM, LPARAM)
{
    return false; // 필요 시 상속 측에서 메시지 소비
}

void Application::BeginFrame(const float clear[4])
{
    g_backbuffer.Bind(g_device);
    g_backbuffer.Clear(g_device, clear);
}

void Application::EndFrame()
{
    g_swapchain.Present(g_desc.vsync ? 1u : 0u);
}
