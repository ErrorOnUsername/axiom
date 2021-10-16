#pragma once

#include <Kernel/Memory/BootloaderMemoryMap.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/KResult.h>

namespace Kernel::Memory {

class MemoryManager {
public:
	static KResult initialize(BootloaderMemoryMap&);

private:
	MemoryManager() = delete;

	static void parse_memory_map(BootloaderMemoryMap&);

	static PhysicalRange m_reported_memory_ranges[11];
};

}
