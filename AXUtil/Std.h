#pragma once

#include <Kernel/Std.h>

template<typename T>
constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);
