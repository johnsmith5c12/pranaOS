/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/StdLibExtraDetails.h>
#include <base/Assertions.h>

template<typename T, typename U>
constexpr auto round_up_to_power_of_two(T value, U power_of_two) requires(IsIntegral<T>&& IsIntegral<U>)
{
    return ((value - 1) & ~(power_of_two - 1)) + power_of_two;
}

namespace std {

template<typename T>
constexpr T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

}

using std::move;

namespace Base::Detail {
template<typename T>
struct _RawPtr {
    using Type = T*;
};
}

namespace Base {

template<class T>
constexpr T&& forward(RemoveReference<T>& param)
{
    return static_cast<T&&>(param);
}

template<class T>
constexpr T&& forward(RemoveReference<T>&& param) noexcept
{
    static_assert(!IsLvalueReference<T>, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(param);
}

template<typename T, typename SizeType = decltype(sizeof(T)), SizeType N>
constexpr SizeType array_size(T (&)[N])
{
    return N;
}

template<typename T>
constexpr T min(const T& a, const IdentityType<T>& b)
{
    return b < a ? b : a;
}

template<typename T>
constexpr T max(const T& a, const IdentityType<T>& b)
{
    return a < b ? b : a;
}

template<typename T>
constexpr T clamp(const T& value, const IdentityType<T>& min, const IdentityType<T>& max)
{
    VERIFY(max >= min);
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

template<typename T, typename U>
constexpr T ceil_div(T a, U b)
{
    static_assert(sizeof(T) == sizeof(U));
    T result = a / b;
    if ((a % b) != 0)
        ++result;
    return result;
}

template<typename T, typename U>
inline void swap(T& a, U& b)
{
    U tmp = move((U&)a);
    a = (T &&) move(b);
    b = move(tmp);
}

template<typename T, typename U = T>
constexpr T exchange(T& slot, U&& value)
{
    T old_value = move(slot);
    slot = forward<U>(value);
    return old_value;
}

template<typename T>
using RawPtr = typename Detail::_RawPtr<T>::Type;

template<typename V>
constexpr decltype(auto) to_underlying(V value) requires(IsEnum<V>)
{
    return static_cast<UnderlyingType<V>>(value);
}

constexpr bool is_constant_evaluated()
{
#if __has_builtin(__builtin_is_constant_evaluated)
    return __builtin_is_constant_evaluated();
#else
    return false;
#endif
}


#define __DEFINE_GENERIC_ABS(type, zero, intrinsic) \
    constexpr type abs(type num)                    \
    {                                               \
        if (is_constant_evaluated())                \
            return num < zero ? -num : num;         \
        else                                        \
            return __builtin_##intrinsic(num);      \
    }

__DEFINE_GENERIC_ABS(int, 0, abs);
__DEFINE_GENERIC_ABS(long, 0l, labs);
__DEFINE_GENERIC_ABS(long long, 0ll, llabs);
#ifndef KERNEL
__DEFINE_GENERIC_ABS(float, 0.0f, fabsf);
__DEFINE_GENERIC_ABS(double, 0.0, fabs);
__DEFINE_GENERIC_ABS(long double, 0.0l, fabsl);
#endif

}

using Base::array_size;
using Base::ceil_div;
using Base::clamp;
using Base::exchange;
using Base::forward;
using Base::is_constant_evaluated;
using Base::max;
using Base::min;
using Base::RawPtr;
using Base::swap;
using Base::to_underlying;