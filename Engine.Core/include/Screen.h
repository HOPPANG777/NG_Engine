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

	int g_WinWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int	g_WinHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	float GetWinWidth() { return static_cast<float>(g_WinWidth); }
	float GetWinHeight() { return static_cast<float>(g_WinHeight); };

	float GetCenterX() { return static_cast<float>(g_WinWidth) * 0.5f; };
	float GetCenterY() { return static_cast<float>(g_WinHeight) * 0.5f; };
	
	DirectX::SimpleMath::Vector3 GetCenter() { return DirectX::SimpleMath::Vector3(GetCenterX(), GetCenterY(), 0); };
	DirectX::SimpleMath::Vector3 GetScreenSize() { return DirectX::SimpleMath::Vector3(GetWinWidth(), GetWinHeight(), 1); };
}