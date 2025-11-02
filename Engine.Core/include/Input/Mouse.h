#pragma once

#include <Windows.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include <directxtk/SimpleMath.h>

namespace NG_Engine
{

	class Mouse
	{
	public:
		static constexpr std::size_t MaxButtonCount = 8;

	public:
		Mouse();
		~Mouse() = default;

		void Init(HWND handle);
		void SetWindowHandle(HWND handle);

		void Update();
		bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

		DirectX::SimpleMath::Vector3 GetMousePos() const { return m_mousePosition; }
		DirectX::SimpleMath::Vector3 GetMoveValue() const { return m_wheelDelta; }

		bool Down(DWORD button) const;
		bool Up(DWORD button) const;
		bool Press(DWORD button) const;
		bool DoubleClick(DWORD button) const;

	private:
		enum class ButtonTransition : std::uint8_t
		{
			None = 0,
			Down,
			Up,
			Press,
			DoubleClick,
		};

		void UpdateDoubleClickState(std::size_t index, ButtonTransition& transition);

		HWND m_windowHandle = nullptr;
		DirectX::SimpleMath::Vector3 m_mousePosition = DirectX::SimpleMath::Vector3::Zero;

		std::array<std::uint8_t, MaxButtonCount> m_buttonState{};
		std::array<std::uint8_t, MaxButtonCount> m_previousButtonState{};
		std::array<ButtonTransition, MaxButtonCount> m_buttonTransitions{};

		DirectX::SimpleMath::Vector3 m_wheelPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_previousWheelPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_wheelDelta = DirectX::SimpleMath::Vector3::Zero;

		DWORD m_doubleClickTime = 0;
		std::array<DWORD, MaxButtonCount> m_buttonClickStart{};
		std::array<int, MaxButtonCount> m_buttonClickCount{};
	};
} // namespace NG_Engine

