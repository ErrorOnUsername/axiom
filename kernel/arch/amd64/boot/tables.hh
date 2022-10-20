#pragma once

#include <libs/ax/types.hh>

namespace Kernel {

extern "C" void* isr_table[];

enum class GDTEntryOffset : u16 {
	NullSegment          = 0x0000,
	KernelCodeSegment    = 0x0008,
	KernelDataSegment    = 0x0010,
	UserspaceDataSegment = 0x0020,
	UserspaceCodeSegment = 0x0018,
};


void init_descriptor_tables();

}
