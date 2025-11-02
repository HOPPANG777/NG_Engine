#include "Core/Platform.h"

using namespace NG_Engine;

bool Platform::CreateWindowClass(const WinDesc& desc)
{
    m_instance = desc.instance;

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
    wc.lpfnWndProc = &Platform::WndProcThunk;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_instance;
    wc.hIcon = desc.icon ? desc.icon : LoadIconW(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = desc.appName.c_str();
    wc.hIconSm = wc.hIcon;

    return RegisterClassExW(&wc) != 0;
}

bool Platform::CreateWindowInstance(const WinDesc& desc, IWinEvents* events)
{
    m_events = events;
    m_width = desc.width;
    m_height = desc.height;

    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = WS_EX_APPWINDOW;

    RECT rc{ 0, 0, LONG(m_width), LONG(m_height) };
    AdjustWindowRectEx(&rc, style, FALSE, exStyle);

    const int scrW = GetSystemMetrics(SM_CXSCREEN);
    const int scrH = GetSystemMetrics(SM_CYSCREEN);
    const int winW = rc.right - rc.left;
    const int winH = rc.bottom - rc.top;
    const int x = (scrW - winW) / 2 + rc.left;
    const int y = (scrH - winH) / 2 + rc.top;

    m_handle = CreateWindowExW(
        exStyle,
        desc.appName.c_str(),     // 클래스명 = 앱명
        desc.appName.c_str(),     // 타이틀
        style,
        x, y, winW, winH,
        nullptr, nullptr,
        m_instance,
        this                      // lpParam → this
    );

    return m_handle != nullptr;
}

void Platform::Show(int cmd)
{
    ShowWindow(m_handle, cmd);
    UpdateWindow(m_handle);
    ShowCursor(TRUE);
}

bool Platform::PumpOnce()
{
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);

        

        DispatchMessageW(&msg);
    }
    return true;
}

void Platform::Destroy()
{
    if (m_handle)
    {
        wchar_t cls[256]{};
        GetClassNameW(m_handle, cls, 256);
        DestroyWindow(m_handle);
        m_handle = nullptr;
        UnregisterClassW(cls, m_instance);
    }
    m_instance = nullptr;
    m_events = nullptr;
}

LRESULT CALLBACK Platform::WndProcThunk(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        auto cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        auto self = static_cast<Platform*>(cs->lpCreateParams);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        // 최초 저장
    }

    auto self = reinterpret_cast<Platform*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
    if (self)
    {
        return self->WndProc(handle, message, wParam, lParam);
    }
    return DefWindowProcW(handle, message, wParam, lParam);
}

LRESULT Platform::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 상위에서 메시지 우선 소비
    if (m_events && m_events->OnMsg(handle, message, wParam, lParam))
        return 0;

    switch (message)
    {
    case WM_SIZE:
    {
        m_minimized = (wParam == SIZE_MINIMIZED);
        const UINT nw = LOWORD(lParam);
        const UINT nh = HIWORD(lParam);
        if (!m_minimized && nw && nh)
        {
            m_width = nw;
            m_height = nh;
            if (m_events) m_events->OnResize(nw, nh);
        }
        return 0;
    }
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(handle, message, wParam, lParam);
    }
}
