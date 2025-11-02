#include "Application.h"
#include "IGraphicsEngine.h"
#include "Common.h"
#include "ConstantColor.h"

using namespace NG_Engine;
using namespace Common;
using namespace ConstantColor;

Application::Application(Graphics::IGraphicsEngine& graphics)
    : m_graphics(graphics)
{

}

bool Application::Init(const AppDesc& desc)
{
    m_desc = desc;

    WinDesc wd{};
    wd.appName = desc.window.appName;
    wd.instance = desc.window.instance;
    wd.width = desc.window.width;
    wd.height = desc.window.height;
    wd.icon = desc.window.icon;

    if (!m_platform.CreateWindowClass(wd)) return false;
    if (!m_platform.CreateWindowInstance(wd, this)) return false;
    m_platform.Show();

    m_desc.window.handle = m_platform.m_handle;
    m_desc.window.width = m_platform.m_width;
    m_desc.window.height = m_platform.m_height;

    m_mouse.Init(m_platform.m_handle);

    Graphics::GraphicsInitDesc graphicsDesc{};
    graphicsDesc.window = m_platform.m_handle;
    graphicsDesc.width = m_platform.m_width;
    graphicsDesc.height = m_platform.m_height;
    graphicsDesc.vsync = desc.vsync;
    graphicsDesc.adapterIndex = desc.adapterIndex;
    if (!m_graphics.Init(graphicsDesc))
    {
        m_platform.Destroy();
        return false;
    }

    bGraphicsInitialized = true;

    m_time.Reset();
    OnInit();
    bRunning = true;
    return true;
}

int Application::Run()
{
    while (bRunning)
    {
        if (!m_platform.PumpOnce())
        {
            bRunning = false;
            break;
        }

        float dt = m_time.Tick();

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
        m_graphics.Shutdown();
        bGraphicsInitialized = false;
    }

    m_platform.Destroy();
}

void Application::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;
    if (!bGraphicsInitialized) return;

    m_desc.window.width = width;
    m_desc.window.height = height;

    m_graphics.Resize(width, height);
}

bool Application::OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool handled = false;
    handled = m_mouse.ProcessMessage(message, wParam, lParam) || handled;

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_UNICHAR:
        handled = m_keyboard.ProcessMessage(message, wParam, lParam) || handled;
        break;
    default:
        break;
    }

    return handled;
}

void Application::BeginFrame(const float clear[4])
{
    if (!bGraphicsInitialized) return;

    m_graphics.BeginFrame(clear);
}

void Application::EndFrame()
{
    if (!bGraphicsInitialized) return;

    m_graphics.EndFrame(m_desc.vsync);
}

void Application::UpdateInputDevices()
{
    m_keyboard.Update();
    m_mouse.Update();
}