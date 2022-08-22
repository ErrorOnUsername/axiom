#pragma once

#include <libs/ax/types.hh>

namespace Kernel {

#define MSR_GS_BASE 0xc000'0101

u64 msr_read(u32 msr)
{
	u32 low  = 0;
	u32 high = 0;
	asm volatile("rdmsr"
	           : "=a"(low), "=d"(high)
	           : "c"(msr));
	return ((u64)high << 32) | low;
}

void msr_write(u32 msr, u64 value)
{
	u32 low  = value & 0xffffffff;
	u32 high = value >> 32;
	asm volatile("wrmsr" ::
	             "a"(low), "d"(high),
	             "c"(msr));
}

}
