#pragma once

#include <AXUtil/StdExtras.h>

// TODO: Implement stl garbage :P

namespace std {

template<typename T>
constexpr T&& forward(AX::Extra::remove_reference<T>& param)
{
	return static_cast<T&&>(param);
}

template<typename T>
constexpr T&& forward(AX::Extra::remove_reference<T>&& param) noexcept
{
	static_assert(!AX::Extra::is_lvalue_reference<T>, "Forwarding an rvalue as an lvalue is invalid!");
	return static_cast<T&&>(param);
}

template<typename T>
constexpr T&& move(T& arg)
{
	return static_cast<T&&>(arg);
}

}
