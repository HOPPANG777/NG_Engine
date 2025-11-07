#pragma once

#include "Screen.h"

namespace UI
{
	int g_WinWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int	g_WinHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	float referenceWidth = static_cast<float>(g_WinWidth);
	float referenceHeight = static_cast<float>(g_WinHeight);

	void SetReferenceResolution(float width, float height){
		referenceWidth = width > 0.0f ? width : 1.0f;
		referenceHeight = height > 0.0f ? height : 1.0f;
	}

	float GetReferenceWidth() { return referenceWidth; }
	float GetReferenceHeight() { return referenceHeight; }

	DirectX::SimpleMath::Vector2 GetReferenceResolution() { 
		return DirectX::SimpleMath::Vector2(referenceWidth, referenceHeight); 
	}

	DirectX::SimpleMath::Vector2 GetUIScale(){
		const float safeWidth = referenceWidth != 0.0f ? referenceWidth : 1.0f;
		const float safeHeight = referenceHeight != 0.0f ? referenceHeight : 1.0f;

		return DirectX::SimpleMath::Vector2(g_WinWidth / safeWidth, g_WinHeight / safeHeight);
	}

	float GetUIScaleX() { return GetUIScale().x; }
	float GetUIScaleY() { return GetUIScale().y; }

	float ApplyUIScaleX(float value) { return value * GetUIScaleX(); }
	float ApplyUIScaleY(float value) { return value * GetUIScaleY(); }

	DirectX::SimpleMath::Vector2 ApplyUIScale(const DirectX::SimpleMath::Vector2& value){
		const DirectX::SimpleMath::Vector2 scale = GetUIScale();
		return DirectX::SimpleMath::Vector2(value.x * scale.x, value.y * scale.y);
	}
}