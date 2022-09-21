#pragma once

#include <libs/ax/types.hh>

namespace Kernel {

inline addr_t read_gs_ptr(usize offset)
{
	addr_t val;
	asm volatile("mov %%gs:%a[off], %[val]"
	            : [val] "=r"(val)
	            : [off] "ir"(offset));
	return val;
}

inline void write_gs_ptr(addr_t val, usize offset)
{
	asm volatile("mov %[val], %%gs:%a[off]"
	            :: [off] "ir"(offset), [val] "r"(val)
	            : "memory");
}

}
