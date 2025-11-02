#pragma once

#include "DX/D3D11Device.h"

#include <imgui.h>
#include <Windows.h>



namespace NG_Engine
{
	struct Graphics::DXDevice;

	class NEGUI
	{
	public:
		NEGUI() = default;
		~NEGUI() = default;

	public:
		void Init(HWND handle, Graphics::DXDevice dx);
		void Shutdown();

		bool ProcessMessage(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
		void Resize();
		
		void Update();
		void Render();

	private:
		void ApplyStyle();

	private:
		bool m_initialized = false;
	
	};
}