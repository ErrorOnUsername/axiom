#pragma once

#include <ax_util/types.hh>
#include <kernel/memory/physical_address.hh>

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
