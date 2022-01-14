#pragma once

#include <ax_util/types.hh>

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
	addr_t            start;
	size_t            size = 0;
};

struct ContiguousPhysicalRange {
	addr_t start;
	size_t size;
};

}
