#pragma once

#include "Screen.h"

namespace UI
{
	//constexpr int WIN_DEFAULT_WIDTH = 1920;
	//constexpr int WIN_DEFAULT_HEIGHT = 1080;

	int g_WinWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int	g_WinHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	static float referenceWidth = g_WinWidth;
	static float referenceHeight = g_WinHeight;

	static void SetReferenceResolution(float width, float height)
	{
		referenceWidth = width > 0.0f ? width : 1.0f;
		referenceHeight = height > 0.0f ? height : 1.0f;
	}

	static float GetReferenceWidth()
	{
		return referenceWidth;
	}

	static float GetReferenceHeight()
	{
		return referenceHeight;
	}

	static DirectX::SimpleMath::Vector2 GetReferenceResolution()
	{
		return DirectX::SimpleMath::Vector2(referenceWidth, referenceHeight);
	}


	static DirectX::SimpleMath::Vector2 GetUIScale()
	{
		const float safeWidth = referenceWidth != 0.0f ? referenceWidth : 1.0f;
		const float safeHeight = referenceHeight != 0.0f ? referenceHeight : 1.0f;

		return DirectX::SimpleMath::Vector2(g_WinWidth / safeWidth, g_WinHeight / safeHeight);
	}

	static float GetUIScaleX()
	{
		return GetUIScale().x;
	}

	static float GetUIScaleY()
	{
		return GetUIScale().y;
	}

	static float ApplyUIScaleX(float value)
	{
		return value * GetUIScaleX();
	}

	static float ApplyUIScaleY(float value)
	{
		return value * GetUIScaleY();
	}

	static DirectX::SimpleMath::Vector2 ApplyUIScale(const DirectX::SimpleMath::Vector2& value)
	{
		const DirectX::SimpleMath::Vector2 scale = GetUIScale();
		return DirectX::SimpleMath::Vector2(value.x * scale.x, value.y * scale.y);
	}
}