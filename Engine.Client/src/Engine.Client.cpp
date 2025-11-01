#include "include/framework.h"
#include "include/Engine.Client.h"

#include <limits>
#include <string>
#include <utility>

#include "Engine.Game/include/EngineAPI.h"
#include "Engine.Graphics/include/DX/D3D11GraphicsEngine.h"
#include "resource.h"

namespace
{
	std::wstring BuildWindowTitle(const wchar_t* baseTitle, const char* version)
	{
		std::wstring title = baseTitle ? baseTitle : L"";
		if (version && *version)
		{
			if (!title.empty())
			{
				title.append(L" ");
			}
			title.append(L"v");
			while (*version)
			{
				title.push_back(static_cast<wchar_t>(*version++));
			}
		}
		return title;
	}
}

EngineClient::EngineClient(NG_Engine::Graphics::IGraphicsEngine& graphics, const std::wstring& baseTitle)
	: NG_Engine::Application(graphics), m_baseTitle(baseTitle)
{
	m_lastPresentedFps = std::numeric_limits<std::uint32_t>::max();
}

void EngineClient::OnInit()
{
	m_titleTimer = 0.0f;
	m_presentedTitle = m_baseTitle;

	if (g_desc.window.handle)
	{
		SetWindowTextW(g_desc.window.handle, m_presentedTitle.c_str());
	}

	std::wstring debugMessage = L"[Engine.Client] Initialized window: ";
	debugMessage += m_presentedTitle;
	debugMessage += L"\n";
	OutputDebugStringW(debugMessage.c_str());
}

void EngineClient::OnUpdate(float dt)
{
	m_titleTimer += dt;
	if (m_titleTimer < 0.25f)
	{
		return;
	}
	m_titleTimer = 0.0f;

	const std::uint32_t fps = g_time.GetFPS();

	std::wstring newTitle = m_baseTitle;
	newTitle.append(L" | FPS: ");
	if (fps == 0)
	{
		newTitle.append(L"--");
	}
	else
	{
		newTitle.append(std::to_wstring(fps));
	}

	if (newTitle != m_presentedTitle && g_desc.window.handle)
	{
		m_presentedTitle = std::move(newTitle);
		SetWindowTextW(g_desc.window.handle, m_presentedTitle.c_str());
	}
}

void EngineClient::OnRender()
{
	// Rendering is handled by subsystems wired through Application::BeginFrame/EndFrame.
	// Place per-frame rendering logic here as the engine evolves.
}

void EngineClient::OnShutdown()
{
	OutputDebugStringW(L"[Engine.Client] Shutdown requested.\n");
}

bool EngineClient::OnMsg(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{

	UNREFERENCED_PARAMETER(handle);
	UNREFERENCED_PARAMETER(lParam);

	if (message == WM_KEYDOWN && wParam == VK_ESCAPE)
	{

		PostQuitMessage(0);
		return true;
	}

	return false;
}


int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	const std::wstring windowTitle = BuildWindowTitle(L"NG_Engine", Engine_GetVersionString());

	if (Engine_Initialize() != Engine::Status::OK)
	{
		MessageBoxW(nullptr, L"Failed to initialize NG_Engine core.", windowTitle.c_str(), MB_OK | MB_ICONERROR);
		return -1;
	}

	int exitCode = -1;

	{
		NG_Engine::Graphics::D3D11GraphicsEngine graphicsEngine;
		NG_Engine::Graphics::IGraphicsEngine& graphicsInterface = graphicsEngine;
		EngineClient clientApp(graphicsInterface, windowTitle);

		NG_Engine::AppDesc desc{};
		desc.window.appName = windowTitle;
		desc.window.instance = hInstance;
		desc.window.width = 1280;
		desc.window.height = 720;
		desc.window.icon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_HO));
		desc.vsync = true;

		if (!clientApp.Initialize(desc))
		{
			MessageBoxW(nullptr, L"Failed to initialize the client application.", windowTitle.c_str(), MB_OK | MB_ICONERROR);
		}
		else
		{
			exitCode = clientApp.Run();
		}
	}

	Engine_Shutdown();

	return exitCode;
}