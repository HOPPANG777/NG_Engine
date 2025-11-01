#pragma once

#include <chrono>
#include <iostream>
#include <Windows.h>
#include <string>

namespace NG_Engine
{
	class Time
	{
	public:
		Time();
		~Time();

	public:
		void Update();

		float GetRealDeltaTime() const { return float(m_deltaTime); }
		float GetDeltaTime() const { return m_scaledDeltaTime; }

	public:
		UINT GetFPS() const { return m_fps; }
		double GetWorldTime() const { return m_worldTime; }

	public:
		void SetTimeScale(float scale) { m_timeScale = scale; }
		float GetTimeScale() const { return m_timeScale; }

		void CheckTime() const;

		// 추가: 엔진 초기화용
		void Reset();

		// 추가: Application 루프 호환용(바로 dt 반환)
		float Tick() { Update(); return GetDeltaTime(); }

	public:
		// UTC 시간 반환
		SYSTEMTIME GetUTCTime() const
		{
			SYSTEMTIME st;
			::GetSystemTime(&st);  // 현재 로컬 시간을 st에 저장
			return st;
		}

		// 컴퓨터 로컬 시간 반환
		SYSTEMTIME GetLocalPCTime() const
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);  // 현재 로컬 시간을 st에 저장
			return st;
		}

	private:
		std::chrono::steady_clock::time_point m_currentTime;
		std::chrono::steady_clock::time_point m_lastTime;
		double m_deltaTime = 0.0;
		double m_scaledDeltaTime = 0.0;

	private:
		UINT m_fps = 0;
		double m_worldTime = 0.0;

	private:
		UINT m_frameCount = 0;
		double m_fpsTimeElapsed = 0.0;

	private:
		float m_scaleDeltaTime = 0.0f;
		float m_timeScale = 1.0f;		// 타임스케일
	};
} // namespace NG_Engine



