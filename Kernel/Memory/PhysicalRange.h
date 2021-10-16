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
	PhysicalAddress address;
	uint64_t size {0};
	PhysicalRangeType type = PhysicalRangeType::Unknown;
};

}
