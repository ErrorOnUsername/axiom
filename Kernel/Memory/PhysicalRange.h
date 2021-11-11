#pragma once

#include <AXUtil/Types.h>
#include <Kernel/Memory/PhysicalAddress.h>

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
	PhysicalAddress address;
	uint64_t size = 0;
};

struct ContiguousPhysicalRange {
	uint64_t start;
	uint64_t end;
};

}
