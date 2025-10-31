#pragma once

#include <Windows.h>
#include <string>

namespace NG_Engine
{
	struct WinDesc
	{
		std::wstring appName = L"";
		HINSTANCE instance = nullptr;
		HWND handle = nullptr;
		UINT width = 0;
		UINT height = 0;
		HICON icon = nullptr;
	};

	struct IWinEvents
	{
		virtual void OnResize(UINT width, UINT height) = 0;
		virtual bool OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual ~IWinEvents() = default;
	};

	class Platform
	{
	public:
		bool CreateWindowClass(const WinDesc& desc);
		bool CreateWindowInstance(const WinDesc& desc, IWinEvents* events);
		
	public:
		void Show(int cmd = SW_SHOWNORMAL);
		bool PumpOnce();			// true: 계속, false: 종료
		void Destroy();

	private:
		static LRESULT CALLBACK WndProcThunk(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
		       LRESULT			WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		IWinEvents*		g_events = nullptr;
		HINSTANCE		g_instance = nullptr;
		HWND			g_handle = nullptr;
		UINT			g_width = 0;
		UINT			g_height = 0;
		bool			g_minimized = false;

	};
}



