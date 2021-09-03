#pragma once

using uint8_t = __UINT8_TYPE__;
using uint16_t = __UINT16_TYPE__;
using uint32_t = __UINT32_TYPE__;
using uint64_t = __UINT64_TYPE__;

using int8_t = __INT8_TYPE__;
using int16_t = __INT16_TYPE__;
using int32_t = __INT32_TYPE__;
using int64_t = __INT64_TYPE__;

using size_t = __SIZE_TYPE__;

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define USED __attribute__((used))
#define ALIGN(x) __attribute__((aligned(x)))
#define SECTION(x) __attribute__((__section__(#x), used))
