#pragma once
/*
	[공통 Include 헤더 예시]
	- 목적: 자주 쓰는 헤더를 한 번만 전처리하여 컴파일 시간을 단축
	- 적용: 각 프로젝트(Include.cpp)에서 /Yc 로 생성, 나머지 .cpp에서 /Yu 로 사용
	- 주의: 자주 변경되는 게임 로직/프로젝트 전용 헤더는 넣지 말 것(전체 재빌드 유발)
*/

// ===============================
// 1) 엔트리 설정(클라이언트 실행파일에서만)
//  - Debug에서 콘솔을 함께 띄워 로그 확인
//  - 라이브러리(Engine/Scripts)에는 적용하지 말 것
// ===============================
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

// ===============================
// 2) Windows 및 CRT 기본 설정
//  - WIN32_LEAN_AND_MEAN: 사용 빈도 낮은 Win32 정의 제외 → 컴파일 가벼움
//  - NOMINMAX: <Windows.h>의 min/max 매크로가 STL과 충돌하는 문제 방지
// ===============================
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Win32
#include <Windows.h>
#include <windowsx.h>     // lParam에서 좌표 추출 등 매크로
#include <commdlg.h>      // 공용 파일 열기/저장 대화상자

// WRL(ComPtr) - COM 객체 수명 자동 관리
#include <wrl/client.h>   // Microsoft::WRL::ComPtr

// CRT/표준 C
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <crtdbg.h>       // 디버그 힙, 누수 체크

// ===============================
// 3) C++ 표준 라이브러리
//  - 빈번 사용 헤더만 수록. 프로젝트 특성에 맞게 추가/삭제
// ===============================
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <exception>
#include <chrono>
#include <random>
#include <mutex>
#include <thread>
#include <future>

// ===============================
// 4) Direct3D 11 / DXGI
//  - dxgi1_6: 어댑터 열거, 출력 정보, VRAM 조회 등 최신 인터페이스
//  - d3d11_1: 일부 편의 인터페이스(ID3D11Device1 등)
//  - d3dcompiler: HLSL 컴파일(개발 환경에서만, 릴리스는 CSO 로딩 권장)
//  - dxgidebug: D3D 디버그 레이어 사용 시
// ===============================
#include <dxgi1_6.h>
#include <d3d11_1.h>
#include <d3d11shader.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")   // 일부 GUID 심볼 정의

// ===============================
// 5) Direct2D / DirectWrite
//  - 2D 벡터 드로잉(D2D), 텍스트 렌더링(DWrite)이 필요할 때만 사용
//  - 순수 D3D11 2D 엔진이면 제외 가능
// ===============================
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <dwrite.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// ===============================
// 6) DirectX 수학/유틸
//  - DirectXMath: SIMD 기반 수학
//  - DirectXColors: 표준 색상 상수
//  - DirectXTK::SimpleMath: 직관적인 Vector/Matrix 래퍼
// ===============================
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <directxtk/SimpleMath.h>

// (DirectXTK 오디오를 쓸 경우 AudioEngine 등)
//#include <directxtk/Audio.h>

// ===============================
// 7) DirectXTex
//  - DDS/PNG/JPG/TGA 등 텍스처 로드/저장, 포맷 변환
//  - WIC 의존성으로 아래 라이브러리 링크 필요
// ===============================
#include <DirectXTex.h>
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

// ===============================
// 8) ImGui
//  - Win32 + DX11 바인딩
//  - ImGui 초기화/프레임 관리 코드는 엔진/툴 구현부에 배치
// ===============================
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// ===============================
// 9) XAudio2
//  - vcpkg의 xaudio2redist 사용 권장(광범위 OS 호환)
//  - DirectXTK Audio를 쓸 때도 내부적으로 XAudio2 사용
// ===============================
#include <xaudio2redist/xaudio2Redist.h>
#pragma comment(lib, "xaudio2_9redist.lib")
// ※ OS 내장 헤더/라이브러리만 사용할 경우: <xaudio2.h> 사용

// ===============================
// 10) 프로젝트 공용 유틸리티
// ===============================
#include "ReadData.h"  // 파일 읽기 헬퍼 등

// ===============================
// 11) 네임스페이스 편의 별칭
//  - 소스에서 짧게 표기
//  - 해당 부분을 cpp 파일 상단에 넣어 사용
// ===============================
/*
	namespace DX = DirectX;
	namespace SM = DirectX::SimpleMath;
	using Microsoft::WRL::ComPtr;
*/


// ===============================
// 12) 디버그 메모리 누수 자동 보고
//  - Debug에서 프로그램 종료 시 메모리 누수 출력
//  - 릴리스 빌드에서는 비활성화
// ===============================
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
struct _LeakChecker {
	_LeakChecker() {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
} _gLeakChecker;
#endif

/*
	[팁]
	- Include에는 "자주 쓰이지만 자주 바뀌지 않는" 헤더만 넣는다.
	- 게임/씬/컴포넌트 등 변경이 잦은 헤더는 PCH에 포함하지 않는다.
	- 모든 .cpp 파일의 첫 줄에 반드시 #include "Include.h"를 넣는다.
	- Include.cpp는 단 하나의 소스에서만 /Yc(pch 생성)로 빌드한다.
*/
