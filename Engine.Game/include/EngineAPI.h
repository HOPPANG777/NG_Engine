#pragma once
/*
    EngineAPI.h
    - NG_Engine 전역에서 공유하는 플랫폼/가시성/버전 매크로 정의
    - 정적/동적 라이브러리 전환 시 소스 수정 없이 빌드 설정만 조정하도록 설계

    [빌드 방법 요약]
    - 정적 .lib로 빌드(기본): 아무 것도 정의하지 않음(ENGINE_DLL 미정의)
    - DLL로 빌드:
        * 엔진 프로젝트(Engine.Core)에만   : ENGINE_DLL + ENGINE_BUILD 정의
        * 엔진을 사용하는 프로젝트에는     : ENGINE_DLL               만 정의
      (VS: 프로젝트 속성 → C/C++ → 전처리기 → 전처리기 정의)
*/

#include <cstdint>

// ===========================
// 1) 플랫폼/컴파일러 감지
// ===========================
#if !defined(_WIN32)
#  error "NG_Engine은 현재 Windows 전용으로 설정되어 있습니다."
#endif

#if defined(_MSC_VER)
#  define ENGINE_COMPILER_MSVC 1
#else
#  define ENGINE_COMPILER_MSVC 0
#endif

// ===========================
// 2) 심볼 가시성 매크로
//    - 정적 라이브러리: 빈 매크로
//    - DLL: export/import 구분
// ===========================
#if defined(ENGINE_DLL)
#  if defined(ENGINE_BUILD)
      // 엔진 DLL을 "빌드"하는 쪽
#    define ENGINE_API __declspec(dllexport)
#  else
      // 엔진 DLL을 "사용"하는 쪽
#    define ENGINE_API __declspec(dllimport)
#  endif
   // 로컬 심볼(내부 전용). MSVC에서는 의미가 약하지만 명시적 구분용
#  define ENGINE_LOCAL
#else
   // 정적 .lib 에서는 가시성 지정 불필요
#  define ENGINE_API
#  define ENGINE_LOCAL
#endif

// ===========================
// 3) 호출 규약/속성 매크로
// ===========================
#ifndef ENGINE_CALL
#  define ENGINE_CALL __cdecl      // 엔진 퍼블릭 C API에 사용할 호출 규약
#endif

#if ENGINE_COMPILER_MSVC
#  define ENGINE_NODISCARD _NODISCARD
#  define ENGINE_FORCEINLINE __forceinline
#else
#  define ENGINE_NODISCARD [[nodiscard]]
#  define ENGINE_FORCEINLINE inline __attribute__((always_inline))
#endif

#if ENGINE_COMPILER_MSVC
#  define ENGINE_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#  define ENGINE_DEPRECATED(msg) __attribute__((deprecated(msg)))
#endif

// ===========================
// 4) 버전 매크로(세맨틱 버전)
//    - 런타임/컴파일타임 호환성 체크에 사용
// ===========================
#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_PATCH 0
#define ENGINE_VERSION_INT   ((ENGINE_VERSION_MAJOR<<16) | (ENGINE_VERSION_MINOR<<8) | (ENGINE_VERSION_PATCH))

// 문자열 매크로 헬퍼
#define ENGINE_STR_HELPER(x) #x
#define ENGINE_STR(x) ENGINE_STR_HELPER(x)
#define ENGINE_VERSION_STRING  \
    ENGINE_STR(ENGINE_VERSION_MAJOR) "." \
    ENGINE_STR(ENGINE_VERSION_MINOR) "." \
    ENGINE_STR(ENGINE_VERSION_PATCH)

// ===========================
// 5) 에러 코드(예시)
//    - 예외를 DLL 경계로 넘기지 않으려면 반환값 기반으로 운영
// ===========================
namespace Engine
{
    enum class Status : std::int32_t
    {
        OK = 0,
        Failed = -1,
        InvalidArgument = -2,
        NotFound = -3,
        NotImplemented = -4,
        DeviceLost = -5,
    };
}

// ===========================
// 6) 퍼블릭 C API (옵션 예시)
//    - DLL 경계 최소화를 위해 C 인터페이스 제공 가능
//    - 정적 .lib 에서도 동일하게 노출되므로 호출측 코드는 동일
// ===========================
#ifdef __cplusplus
extern "C" {
#endif

    // 엔진 런타임 버전 조회
    ENGINE_API ENGINE_NODISCARD std::uint32_t ENGINE_CALL Engine_GetVersionInt();
    ENGINE_API ENGINE_NODISCARD const char* ENGINE_CALL Engine_GetVersionString();

    // 엔진 초기화/종료의 최소 C API 예시(필요 시 확장)
    ENGINE_API Engine::Status ENGINE_CALL Engine_Initialize();
    ENGINE_API void           ENGINE_CALL Engine_Shutdown();

#ifdef __cplusplus
} // extern "C"
#endif

// ===========================
// 7) C++ 클래스에 적용하는 법(가이드)
//    - DLL일 때만 내보내기 심볼 부여
//    - STL을 멤버로 직접 노출하지 말고 pImpl 권장(DLL 경계 ABI 이슈 회피)
// ===========================
/*
namespace Engine {

class ENGINE_API Renderer;           // 선언부에 ENGINE_API 부여
class ENGINE_API Texture;

class ENGINE_API Application
{
public:
    Application() noexcept;
    ~Application() noexcept;

    Engine::Status Initialize() noexcept;
    void           Run();
    void           Shutdown() noexcept;

private:
    struct Impl;          // pImpl
    Impl* p_;             // 소유/수명은 동일 모듈에서 관리
};

} // namespace Engine
*/

// ===========================
// 8) MSVC 경고 정리(선택)
//    - 4251: STL 멤버를 내보낼 때 경고. pImpl 권장. 필요시 아래 사용.
// ===========================
#if ENGINE_COMPILER_MSVC
#  pragma warning(disable:4251) // 'std::xxx' needs to have dll-interface
#  pragma warning(disable:4275) // non - DLL-interface class 'X' used as base
#endif

// ===========================
// 9) 디버그 모드 콘솔 출력
//    - 디버그 모드에서 wWinMainCRTStartup을 진입점으로 설정하여 콘솔 창을 노출
// ===========================
#ifdef _DEBUG
#  pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif