#include "Input/Mouse.h"

#include <Windows.h>
#include <windowsx.h>

#include <imgui.h>

using namespace NG_Engine;
using DirectX::SimpleMath::Vector3;

namespace
{
    inline ImGuiIO& GetImGuiIO()
    {
        return ImGui::GetIO();
    }

    constexpr std::uint8_t GetAsyncButtonState(int virtualKey)
    {
        return (::GetAsyncKeyState(virtualKey) & 0x8000) != 0 ? 1u : 0u;
    }
}

Mouse::Mouse()
{
    Init(nullptr);
}

void Mouse::Init(HWND handle)
{
    m_windowHandle = handle;

    m_buttonState.fill(0);
    m_previousButtonState.fill(0);
    m_buttonTransitions.fill(ButtonTransition::None);

    m_wheelPosition = Vector3::Zero;
    m_previousWheelPosition = Vector3::Zero;
    m_wheelDelta = Vector3::Zero;

    m_doubleClickTime = ::GetDoubleClickTime();
    const DWORD now = ::GetTickCount64();
    m_buttonClickStart.fill(now);
    m_buttonClickCount.fill(0);
}

void Mouse::SetWindowHandle(HWND handle)
{
    m_windowHandle = handle;
}

void Mouse::Update()
{
    m_previousButtonState = m_buttonState;
    m_previousWheelPosition = m_wheelPosition;
    m_wheelDelta = Vector3::Zero;

    ImGuiIO& io = GetImGuiIO();
    if (io.WantCaptureMouse)
    {
        m_buttonState.fill(0);

        for (std::size_t button = 0; button < MaxButtonCount; ++button)
        {
            const std::uint8_t previous = m_previousButtonState[button];
            m_buttonTransitions[button] = previous != 0 ? ButtonTransition::Up : ButtonTransition::None;
        }

        m_mousePosition = Vector3::Zero;
        m_wheelPosition = Vector3::Zero;
        return;
    }

    m_buttonState[0] = GetAsyncButtonState(VK_LBUTTON);
    m_buttonState[1] = GetAsyncButtonState(VK_RBUTTON);
    m_buttonState[2] = GetAsyncButtonState(VK_MBUTTON);

    for (std::size_t button = 0; button < MaxButtonCount; ++button)
    {
        const std::uint8_t previous = m_previousButtonState[button];
        const std::uint8_t current = m_buttonState[button];
        ButtonTransition transition = ButtonTransition::None;

        if (previous == 0u && current == 1u)
        {
            transition = ButtonTransition::Down;
        }
        else if (previous == 1u && current == 0u)
        {
            transition = ButtonTransition::Up;
        }
        else if (previous == 1u && current == 1u)
        {
            transition = ButtonTransition::Press;
        }

        UpdateDoubleClickState(button, transition);
        m_buttonTransitions[button] = transition;
    }

    POINT cursor{};
    if (::GetCursorPos(&cursor) != 0)
    {
        if (m_windowHandle != nullptr)
        {
            ::ScreenToClient(m_windowHandle, &cursor);
        }

        m_mousePosition.x = static_cast<float>(cursor.x);
        m_mousePosition.y = static_cast<float>(cursor.y);
        m_wheelPosition.x = m_mousePosition.x;
        m_wheelPosition.y = m_mousePosition.y;
    }

    m_wheelDelta = m_wheelPosition - m_previousWheelPosition;

    m_previousButtonState = m_buttonState;
    m_previousWheelPosition = m_wheelPosition;
}

bool Mouse::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    const ImGuiIO& io = GetImGuiIO();
    bool handled = false;

    switch (message)
    {
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    {
        m_mousePosition.x = static_cast<float>(GET_X_LPARAM(lParam));

        if (m_windowHandle != nullptr)
        {
            RECT rect{};
            if (::GetClientRect(m_windowHandle, &rect) != 0)
            {
                const float height = static_cast<float>(rect.bottom - rect.top);
                m_mousePosition.y = height - static_cast<float>(GET_Y_LPARAM(lParam));
            }
            else
            {
                m_mousePosition.y = static_cast<float>(GET_Y_LPARAM(lParam));
            }
        }
        else
        {
            m_mousePosition.y = static_cast<float>(GET_Y_LPARAM(lParam));
        }
        handled = io.WantCaptureMouse;
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const short wheelDelta = static_cast<short>(HIWORD(wParam));
        m_wheelPosition.z += static_cast<float>(wheelDelta);
        handled = io.WantCaptureMouse;
        break;
    }
    default:
        return false;
    }

    return handled;
}

bool Mouse::Down(DWORD button) const
{
    if (button >= MaxButtonCount)
    {
        return false;
    }

    return m_buttonTransitions[button] == ButtonTransition::Down;
}

bool Mouse::Up(DWORD button) const
{
    if (button >= MaxButtonCount)
    {
        return false;
    }

    return m_buttonTransitions[button] == ButtonTransition::Up;
}

bool Mouse::Press(DWORD button) const
{
    if (button >= MaxButtonCount)
    {
        return false;
    }

    return m_buttonTransitions[button] == ButtonTransition::Press;
}

bool Mouse::DoubleClick(DWORD button) const
{
    if (button >= MaxButtonCount)
    {
        return false;
    }

    return m_buttonTransitions[button] == ButtonTransition::DoubleClick;
}

void Mouse::UpdateDoubleClickState(std::size_t index, ButtonTransition& transition)
{
    const DWORD now = ::GetTickCount64();

    if (transition == ButtonTransition::Down)
    {
        if (m_buttonClickCount[index] == 1 && (now - m_buttonClickStart[index]) >= m_doubleClickTime)
        {
            m_buttonClickCount[index] = 0;
        }

        if (m_buttonClickCount[index] == 0)
        {
            m_buttonClickStart[index] = now;
        }

        ++m_buttonClickCount[index];
    }
    else if (transition == ButtonTransition::Up)
    {
        if (m_buttonClickCount[index] == 1)
        {
            if ((now - m_buttonClickStart[index]) >= m_doubleClickTime)
            {
                m_buttonClickCount[index] = 0;
            }
        }
        else if (m_buttonClickCount[index] == 2)
        {
            if ((now - m_buttonClickStart[index]) <= m_doubleClickTime)
            {
                transition = ButtonTransition::DoubleClick;
            }

            m_buttonClickCount[index] = 0;
        }
    }
    else if (transition == ButtonTransition::None)
    {
        if (m_buttonClickCount[index] > 0 && (now - m_buttonClickStart[index]) >= m_doubleClickTime)
        {
            m_buttonClickCount[index] = 0;
        }
    }
}