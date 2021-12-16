#include "MemoryManager.h"

#include <AXUtil/Helpers.h>
#include <AXUtil/List.h>
#include <Kernel/Arch/x86_64/Boot/boot_stivale.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>
#include <Kernel/Memory/PhysicalRange.h>
#include <Kernel/k_debug.h>

namespace Kernel::Memory {

AX::List<PhysicalRange>           MemoryManager::m_reported_physical_ranges;
AX::List<ContiguousPhysicalRange> MemoryManager::m_contiguous_physical_ranges;

void MemoryManager::initialize(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
}

void MemoryManager::parse_memory_map(BootloaderMemoryMap& memory_map)
{
	m_reported_physical_ranges.ensure_capacity(memory_map.length);
	for(uint64_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry& current_entry = memory_map.entries[i];
		klogf(LogLevel::Info, "PhysicalRange: { address: %xl, length: %xl, type: %s }"
		    , current_entry.address
		    , current_entry.size
		    , memory_map_type_as_string(current_entry.type));

		switch(current_entry.type) {
			case MemoryRegionType::Usable:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Usable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::Reserved:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Reserved,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::ACPIReclaimable:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::ACPIReclaimable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::ACPINVS:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::ACPINVS,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::BadMemory:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::BadMemory,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::BootloaderReclaimable:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::BootloaderReclaimable,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::KernelAndModules:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::KernelAndModules,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			case MemoryRegionType::Framebuffer:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Framebuffer,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
			default:
				m_reported_physical_ranges.append(PhysicalRange {
					.type = PhysicalRangeType::Unknown,
					.address = current_entry.address,
					.size = current_entry.size
				});
				break;
		}

		if(current_entry.type != MemoryRegionType::Usable)
			continue;

		// stivale2 guarantees that all sections marked "USABLE" are page-aligned,
		// but we still check so that, if we ever use a different bootloader that
		// doesn't have this behavior, it won't be a problem.
		uint16_t addr_adjustment = (PAGE_SIZE - (current_entry.address % PAGE_SIZE)) % PAGE_SIZE;
		current_entry.address += addr_adjustment;
		current_entry.size    -= addr_adjustment;
		current_entry.size    -= current_entry.size % PAGE_SIZE;

		klogf(LogLevel::Warning, "Adjusted range to: { address: %xl, size: %xl, type: %s }"
		    , current_entry.address
		    , current_entry.size
		    , memory_map_type_as_string(current_entry.type));

		if(current_entry.size < PAGE_SIZE) {
			klogf(LogLevel::Warning, "The bootloader reported a PhysicalRange [%xl - %xl] with a size less than %u (one page)! Skipping it..."
			    , current_entry.address
			    , current_entry.address + (current_entry.size - 1)
			    , PAGE_SIZE);
			continue;
		}

		// Walk the pages in each Usable section to find all contiguous regions
		for(uint64_t page_addr = current_entry.address;
		             page_addr < (current_entry.address + current_entry.size);
		             page_addr += PAGE_SIZE) {
			if(!m_contiguous_physical_ranges.is_empty() && m_contiguous_physical_ranges.last().end == page_addr - PAGE_SIZE) {
				// If out list isn't empty, and the page we're looking at is the next sequential
				// page after the last one we looked at, then we make that the new end to the
				// current contiguous range.
				m_contiguous_physical_ranges.last().end = page_addr;
			} else {
				m_contiguous_physical_ranges.append(ContiguousPhysicalRange {
					.start = page_addr,
					.end   = page_addr
				});
			}

		}
	}
}

void MemoryManager::initialize_pages()
{ }

}
