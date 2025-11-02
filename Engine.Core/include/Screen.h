#pragma once

#include <directxtk/SimpleMath.h>
#include <cstdint>
#include <Windows.h>
#include "Application.h"

struct Resolution
{
	int width = 0;
	int height = 0;
	float Aspect() const { return (height != 0) ? (float)width / (float)height : 0.0f; }
};

struct Viewport
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	float scale = 1.0f;
};

namespace Screen
{
	struct DisplayState
	{
		Resolution Window;
		Resolution Render;

		float dpiScale = 1.0f;
		bool bScreen = false;
	};

	//constexpr int WIN_DEFAULT_WIDTH = 1920;
	//constexpr int WIN_DEFAULT_HEIGHT = 1080;

	int g_WinWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int	g_WinHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	static UINT GetWinWidth() { return static_cast<UINT>(g_WinWidth); }
	static UINT GetWinHeight() { return static_cast<UINT>(g_WinHeight); };
	static float GetCenterX() { return g_WinWidth * 0.5f; };
	static float GetCenterY() { return g_WinHeight * 0.5f; };
	static DirectX::SimpleMath::Vector3 GetCenter() { return DirectX::SimpleMath::Vector3(GetCenterX(), GetCenterY(), 0); };
	static DirectX::SimpleMath::Vector3 GetScreenSize() { return DirectX::SimpleMath::Vector3(g_WinWidth, g_WinHeight, 1); };
}