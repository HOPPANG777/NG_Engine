#pragma once

#include <cstdint>
#include <string>

#include "Engine.Game/include/Application.h"

class EngineClient final : public NG_Engine::Application
{
public:
	EngineClient(NG_Engine::Graphics::IGraphicsEngine& graphics, std::wstring baseTitle);

protected:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnRender() override;
	void OnShutdown() override;
	bool OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	std::wstring m_baseTitle;
	std::wstring m_presentedTitle;
	float m_titleTimer = 0.0f;
	std::uint32_t m_lastPresentedFps = 0;

};
