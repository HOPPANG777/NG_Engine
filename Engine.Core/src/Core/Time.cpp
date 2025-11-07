#include "Core/Time.h"

using namespace NG_Engine;

Time::Time()
{
	m_currentTime = std::chrono::steady_clock::now();
	m_lastTime = m_currentTime;
}

Time::~Time() {}

void Time::Reset()
{
	m_currentTime = std::chrono::steady_clock::now();
	m_lastTime = m_currentTime;
	m_deltaTime = 0.0;
	m_scaledDeltaTime = 0.0;
	m_worldTime = 0.0;
	m_fps = 0;
	m_frameCount = 0;
	m_fpsTimeElapsed = 0.0;
}

void Time::Update()
{
	m_lastTime = m_currentTime;

	m_currentTime = std::chrono::steady_clock::now();

	std::chrono::duration<double> delta = m_currentTime - m_lastTime;
	m_deltaTime = delta.count();

	m_worldTime += m_deltaTime;

	m_fpsTimeElapsed += m_deltaTime;
	++m_frameCount;

	if (m_fpsTimeElapsed >= 1.0)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_fpsTimeElapsed = 0.0;

#if defined(_DEBUG) || defined(ENGINE_DEBUG)
		CheckTime();
#endif
	}

	m_scaledDeltaTime = m_deltaTime * m_timeScale;
}

void Time::CheckTime() const
{
		std::cout << "FPS : " << m_fps << ", DeltaTime : "
			<< m_scaledDeltaTime << ", WorldTime : " << m_worldTime << '\n';	
}
