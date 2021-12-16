#pragma once

#include <AXUtil/Helpers.h>
#include <AXUtil/List.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/Memory/PageTable.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>

namespace Kernel::Memory {

class MemoryManager {
	MAKE_NONCOPYABLE(MemoryManager);
	MAKE_NONMOVEABLE(MemoryManager);

public:
	MemoryManager() = delete;

	static void initialize(BootloaderMemoryMap&);
	static void parse_memory_map(BootloaderMemoryMap&);
	static void initialize_pages();

private:
	static AX::List<PhysicalRange>           m_reported_physical_ranges;
	static AX::List<ContiguousPhysicalRange> m_contiguous_physical_ranges;
};

}
