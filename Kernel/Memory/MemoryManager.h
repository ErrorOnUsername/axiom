#pragma once

#include <AXUtil/Helpers.h>
#include <AXUtil/List.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>
#include <Kernel/KResult.h>

namespace Kernel::Memory {

struct MemoryManager {
	MAKE_NONCOPYABLE(MemoryManager);
	MAKE_NONMOVEABLE(MemoryManager);
	MemoryManager() = delete;

	static AX::List<PhysicalRange>           reported_physical_ranges;
	static AX::List<ContiguousPhysicalRange> contiguous_physical_ranges;

	static KResult initialize(BootloaderMemoryMap&);
	static void    parse_memory_map(BootloaderMemoryMap&);
};

}
