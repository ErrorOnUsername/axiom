#include "MemoryManager.h"

#include <AXUtil/Helpers.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/KResult.h>
#include <Kernel/k_stdio.h>

namespace Kernel::Memory {

PhysicalRange MemoryManager::m_reported_memory_ranges[11];

KResult MemoryManager::initialize(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
	return KResult::Success;
}

void MemoryManager::parse_memory_map(BootloaderMemoryMap& memory_map)
{
	for(uint64_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry& current_entry = memory_map.entries[i];
		k_printf("[MM] | MemoryRegion: address: %xl, length: %xl, type: %s\n", current_entry.address, current_entry.size, memory_map_type_as_string(current_entry.type));

		switch(current_entry.type) {
			case MemoryRegionType::Usable:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::Usable
				};
				break;
			case MemoryRegionType::Reserved:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::Reserved
				};
				break;
			case MemoryRegionType::ACPIReclaimable:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::ACPIReclaimable
				};
				break;
			case MemoryRegionType::ACPINVS:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::ACPINVS
				};
				break;
			case MemoryRegionType::BadMemory:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::BadMemory
				};
				break;
			case MemoryRegionType::BootloaderReclaimable:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::BootloaderReclaimable
				};
				break;
			case MemoryRegionType::KernelAndModules:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::KernelAndModules
				};
				break;
			case MemoryRegionType::Framebuffer:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::Framebuffer
				};
				break;
			default:
				m_reported_memory_ranges[i] = PhysicalRange {
					.address = current_entry.address,
					.size = current_entry.size,
					.type = PhysicalRangeType::Unknown
				};
				break;
		}

		if(current_entry.type != MemoryRegionType::Usable)
			continue;

		// Stivale2 guarentees that all sections marked "USABLE" are page aligned,
		// but we still check so that, if we ever use a different bootloader that
		// doesn't have this behavior, it won't be a problem.

		uint16_t displacement = current_entry.address % PAGE_SIZE;
		current_entry.address = ADDR_PAGE_ALIGN_UP(current_entry.address);
		current_entry.size = ADDR_PAGE_ALIGN_DOWN(current_entry.size) - displacement;

		if(current_entry.size < PAGE_SIZE) {
			k_printf("[MM] | The bootloader reported a PhysicalRegion [%xl - %xl] with a size less than %d (one page)! Skipping it...\n", current_entry.address, current_entry.address + (current_entry.size - 1), PAGE_SIZE);
			continue;
		}

		// Walk the pages in each PhysicalRegion to find all contiguous regions
		// for(uint64_t page_address = current_entry.address; page_address < (current_entry.address + current_entry.size); i += PAGE_SIZE) { }
	}
}

}
