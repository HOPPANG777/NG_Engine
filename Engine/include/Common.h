#pragma once
// 외부 라이브러리, OS 헤더, 그래픽스 API 의존 관련은 무조건 금지. 유틸의 최소 단위여야 함. 그 말인즉슨, 표준 헤더를 제외한 의존성이 0이어야 함.

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <utility>
#include <type_traits>
#include <limits>

namespace common
{
    // ------------------ 기본형 타입 별칭 ------------------
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    // ------------------ 컴파일타임 상수 ------------------
    constexpr float  PI = 3.14159265358979323846f;
    constexpr double E = 2.71828182845904523536;
    constexpr float  EPS = 1e-6f;

    // ------------------ 수학 유틸 ------------------
    template<typename T>
    constexpr T clamp(T v, T lo, T hi)
    {
        return (v < lo) ? lo : (v > hi) ? hi : v;
    }

    template<typename T>
    constexpr T lerp(T a, T b, float t)
    {
        return a + (b - a) * t;
    }

    template<typename T>
    constexpr bool nearly_equal(T a, T b, T eps = std::numeric_limits<T>::epsilon() * 10)
    {
        return (a > b ? a - b : b - a) <= eps;
    }

    // ------------------ 범위 검사 ------------------
    template<typename T>
    constexpr bool in_range(T v, T lo, T hi)
    {
        return v >= lo && v <= hi;
    }

    // ------------------ 어서션 래퍼 ------------------
#ifdef _DEBUG
#define COMMON_ASSERT(expr) assert(expr)
#else
#define COMMON_ASSERT(expr) ((void)0)
#endif
    // ------------------ 런타임 검사 ------------------
    namespace detail
    {
        inline void check_fail(const char* expr, const char* file, int line)
        {
            std::fprintf(stderr, "[Check Failed] %s\n  at %s(%d)\n", expr, file, line);
            std::fflush(stderr);
            assert(false);
        }
    }

    // 사용 예: CHECK(ptr != nullptr);
#ifdef _DEBUG
#define CHECK(expr) \
    do { if (!(expr)) ::common::detail::check_fail(#expr, __FILE__, __LINE__); } while (0)
#else
#define CHECK(expr) ((void)0)
#endif
}