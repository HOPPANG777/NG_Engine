#pragma once

#include <Windows.h>

#include <array>
#include <cstddef>
#include <cstdint>

//namespace ImGui
//{
//	struct ImGuiIO;
//}

namespace NG_Engine
{
	class Keyboard
	{
	public:
		static constexpr std::size_t MaxKeyCount = 256;

	public:
		Keyboard() = default;
		~Keyboard() = default;

		void Update();

		bool Down(DWORD key) const;
		bool Up(DWORD key) const;
		bool Press(DWORD key) const;

	private:
		enum class KeyTransition : std::uint8_t
		{
			None = 0,
			Down,
			Up,
			Press,
		};

		std::array<std::uint8_t, MaxKeyCount> m_keyState{};
		std::array<std::uint8_t, MaxKeyCount> m_previousKeyState{};
		std::array<KeyTransition, MaxKeyCount> m_keyTransitions{};

	};
} // namespace NG_Engine