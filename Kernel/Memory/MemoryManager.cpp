#include "MemoryManager.h"

#include <AXUtil/Helpers.h>
#include <AXUtil/List.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/KResult.h>
#include <Kernel/k_debug.h>

namespace Kernel::Memory {

AX::List<PhysicalRange>           MemoryManager::reported_physical_ranges;
AX::List<ContiguousPhysicalRange> MemoryManager::contiguous_physical_ranges;

KResult MemoryManager::initialize(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
	return KResult::Success;
}

void MemoryManager::parse_memory_map(BootloaderMemoryMap& memory_map)
{
	for(uint64_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry& current_entry = memory_map.entries[i];
		klogf(LogModeInfo | LogModeMemoryManager, "PhysicalRange: { address: %xl, length: %xl, type: %s }", current_entry.address, current_entry.size, memory_map_type_as_string(current_entry.type));

		switch(current_entry.type) {
			case MemoryRegionType::Usable:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Usable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::Reserved:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Reserved,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::ACPIReclaimable:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::ACPIReclaimable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::ACPINVS:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::ACPINVS,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::BadMemory:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::BadMemory,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::BootloaderReclaimable:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::BootloaderReclaimable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::KernelAndModules:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::KernelAndModules,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::Framebuffer:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Framebuffer,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			default:
				reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Unknown,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
		}

		if(current_entry.type != MemoryRegionType::Usable)
			continue;

		// stivale2 guarentees that all sections marked "USABLE" are page-aligned,
		// but we still check so that, if we ever use a different bootloader that
		// doesn't have this behavior, it won't be a problem.
		//uint16_t addr_adjustment = PAGE_SIZE - (current_entry.address % PAGE_SIZE);
		//current_entry.address += addr_adjustment;
		//current_entry.size    -= addr_adjustment;
		//current_entry.size    -= current_entry.size % PAGE_SIZE;

		if(current_entry.size < PAGE_SIZE) {
			klogf(LogModeWarning | LogModeMemoryManager, "The bootloader reported a PhysicalRegion [%xl - %xl] with a size less than %u (one page)! Skipping it..."
			    , current_entry.address
			    , current_entry.address + (current_entry.size - 1)
			    , PAGE_SIZE);
			continue;
		}

		// Walk the pages in each Usable section to find all contiguous regions
		for(uint64_t page_addr = current_entry.address;
		             page_addr < (current_entry.address + current_entry.size);
		             page_addr += PAGE_SIZE) {
			if(!contiguous_physical_ranges.is_empty() && contiguous_physical_ranges.last().end == page_addr - PAGE_SIZE) {
				// If out list isn't empty, and the page we're looking at is the next sequential
				// page after the last one we looked at, then we make that the new end to the
				// current contiguous range.
				contiguous_physical_ranges.last().end = page_addr;
			} else {
				contiguous_physical_ranges.append(ContiguousPhysicalRange {
					.start = page_addr,
					.end   = page_addr
				});
			}

		}

	}

	for(size_t i = 0; i < contiguous_physical_ranges.count; i++) {
		klogf(LogModeInfo | LogModeMemoryManager, "ContiguousPhysicalRange { start: %xl, end: %xl }"
		    , contiguous_physical_ranges[i].start
		    , contiguous_physical_ranges[i].end);
	}
}

}
