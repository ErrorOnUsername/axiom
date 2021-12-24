#pragma once

#include <AXUtil/Types.h>
#include <kernel/memory/physical_address.h>

namespace Kernel::Memory {

enum class PhysicalRangeType {
	Unknown,
	Usable,
	Reserved,
	ACPIReclaimable,
	ACPINVS,
	BadMemory,
	BootloaderReclaimable,
	KernelAndModules,
	Framebuffer,
};

struct PhysicalRange {
	PhysicalRangeType type;
	PhysicalAddress start;
	uint64_t size = 0;
};

struct ContiguousPhysicalRange {
	PhysicalAddress start;
	size_t size;
};

}
