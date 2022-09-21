#pragma once

// TODO: Verify that these work for other compilers too, just in case we want
//       to support them.
using u8  = __UINT8_TYPE__;
using u16 = __UINT16_TYPE__;
using u32 = __UINT32_TYPE__;
using u64 = __UINT64_TYPE__;

using i8  = __INT8_TYPE__;
using i16 = __INT16_TYPE__;
using i32 = __INT32_TYPE__;
using i64 = __INT64_TYPE__;

using size_t    = __SIZE_TYPE__;
using usize     = __SIZE_TYPE__;
using uintptr_t = __UINTPTR_TYPE__;
using ptrdiff_t = __PTRDIFF_TYPE__;

// BUG: This doesn't work for anything other than 64-bit
using addr_t = u64;

typedef void (*AXCallback)();
