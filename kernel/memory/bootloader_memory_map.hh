#pragma once

#include <ax_util/types.hh>

namespace Kernel::Memory {

enum MemoryRegionType : uint32_t {
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
	addr_t   address;
	uint64_t size;
	uint32_t type;
};

struct BootloaderMemoryMap {
	BootloaderMemoryMapEntry* entries;
	uint64_t                  length;
};

char const* memory_map_type_as_string(uint32_t type);

}
