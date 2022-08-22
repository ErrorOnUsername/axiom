#pragma once

#include <libs/ax/types.hh>

namespace Kernel::Memory {

enum MemoryRegionType : u32 {
	Usable                = 1,
	Reserved              = 2,
	ACPIReclaimable       = 3,
	ACPINVS               = 4,
	BadMemory             = 5,
	BootloaderReclaimable = 0x1000,
	KernelAndModules      = 0x1001,
	Framebuffer           = 0x1002,
};

struct BootloaderMemoryMapEntry {
	addr_t address;
	u64    size;
	u32    type;
};

struct BootloaderMemoryMap {
	BootloaderMemoryMapEntry* entries;
	u64                       length;
};

char const* memory_map_type_as_string(u32 type);

}
