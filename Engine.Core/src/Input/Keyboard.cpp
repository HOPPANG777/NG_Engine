#include "Input/Keyboard.h"

#include <Windows.h>

#include <imgui.h>

using namespace NG_Engine;

namespace
{
    inline ImGuiIO& GetImGuiIO()
    {
        return ImGui::GetIO();
    }
}

void Keyboard::Update()
{
    m_previousKeyState = m_keyState;

    ImGuiIO& io = GetImGuiIO();
    if (io.WantCaptureKeyboard || io.WantTextInput)
    {
        m_keyState.fill(0);

        for (std::size_t key = 0; key < MaxKeyCount; ++key)
        {
            const std::uint8_t previous = m_previousKeyState[key];
            m_keyTransitions[key] = previous != 0 ? KeyTransition::Up : KeyTransition::None;
        }

        return;
    }

    if (::GetKeyboardState(m_keyState.data()) == 0)
    {
        for (std::size_t key = 0; key < MaxKeyCount; ++key)
        {
            m_keyTransitions[key] = KeyTransition::None;
            m_keyState[key] = 0;
        }

        return;
    }

    for (std::size_t key = 0; key < MaxKeyCount; ++key)
    {
        std::uint8_t& state = m_keyState[key];
        state = (state & 0x80u) != 0u ? 1u : 0u;

        const std::uint8_t previous = m_previousKeyState[key];

        if (previous == 0u && state == 1u)
        {
            m_keyTransitions[key] = KeyTransition::Down;
        }
        else if (previous == 1u && state == 0u)
        {
            m_keyTransitions[key] = KeyTransition::Up;
        }
        else if (previous == 1u && state == 1u)
        {
            m_keyTransitions[key] = KeyTransition::Press;
        }
        else
        {
            m_keyTransitions[key] = KeyTransition::None;
        }
    }
}

bool Keyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    const ImGuiIO& io = GetImGuiIO();
    if (io.WantCaptureKeyboard || io.WantTextInput)
    {
        return true;
    }

    return false;
}

bool Keyboard::Down(DWORD key) const
{
    if (key >= MaxKeyCount)
    {
        return false;
    }

    return m_keyTransitions[key] == KeyTransition::Down;
}

bool Keyboard::Up(DWORD key) const
{
    if (key >= MaxKeyCount)
    {
        return false;
    }

    return m_keyTransitions[key] == KeyTransition::Up;
}

bool Keyboard::Press(DWORD key) const
{
    if (key >= MaxKeyCount)
    {
        return false;
    }

    return m_keyTransitions[key] == KeyTransition::Press;
}
