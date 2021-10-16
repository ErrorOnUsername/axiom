#pragma once

#include <AXUtil/Types.h>

namespace Kernel {

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
	uint64_t address;
	uint64_t size;
	uint32_t type;
};

struct BootloaderMemoryMap {
	uint64_t length;
	BootloaderMemoryMapEntry* entries;
};

}
