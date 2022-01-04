#include "memory_manager.hh"

#include <ax_util/helpers.hh>
#include <ax_util/list.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/physical_range.hh>
#include <kernel/k_debug.hh>

namespace Kernel::Memory {

static AX::List<PhysicalRange>           reported_physical_ranges;
static AX::List<ContiguousPhysicalRange> contiguous_physical_ranges;
static AX::List<ContiguousPhysicalRange> contiguous_reserved_physical_ranges;

static void locate_reserved_physical_ranges()
{
	ContiguousPhysicalRange contiguous_range { };
	PhysicalRange           current_range    { };

	for(size_t i = 0; i < reported_physical_ranges.count; i++) {
		current_range = reported_physical_ranges[i];

		if(current_range.type != PhysicalRangeType::Reserved) {
			if(contiguous_range.start.is_null())
				continue;

			contiguous_reserved_physical_ranges.append(ContiguousPhysicalRange {
				.start = contiguous_range.start,
				.size  = current_range.start.addr - contiguous_range.start.addr
			});

			contiguous_range.start = 0;
			continue;
		}

		if(!contiguous_range.start.is_null())
			continue;

		contiguous_range.start = current_range.start;
	}

	if(reported_physical_ranges.last().type != PhysicalRangeType::Reserved)
		return;

	if(contiguous_range.start.is_null())
		return;

	contiguous_reserved_physical_ranges.append(ContiguousPhysicalRange {
		.start = contiguous_range.start,
		.size  = (reported_physical_ranges.last().start.addr - contiguous_range.start.addr) + reported_physical_ranges.last().size
	});
}

static void initialize_pages()
{
}

static void parse_memory_map(BootloaderMemoryMap& memory_map)
{
	reported_physical_ranges.ensure_capacity(memory_map.length);
	for(uint64_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry& current_entry = memory_map.entries[i];

		klogf(LogLevel::Info, "PhysicalRange: { address: %xl, length: %xl, type: %s }"
		    , current_entry.address
		    , current_entry.size
		    , memory_map_type_as_string(current_entry.type));

		switch(current_entry.type) {
			case MemoryRegionType::Usable:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::Usable,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::Reserved:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::Reserved,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::ACPIReclaimable:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::ACPIReclaimable,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::ACPINVS:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::ACPINVS,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::BadMemory:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::BadMemory,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::BootloaderReclaimable:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::Usable,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::KernelAndModules:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::KernelAndModules,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			case MemoryRegionType::Framebuffer:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::Framebuffer,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
			default:
				reported_physical_ranges.append(PhysicalRange {
					.type  = PhysicalRangeType::Unknown,
					.start = current_entry.address,
					.size  = current_entry.size
				});
				break;
		}

		if(current_entry.type != MemoryRegionType::Usable && current_entry.type != MemoryRegionType::BootloaderReclaimable)
			continue;

		// stivale2 guarantees that all sections marked "USABLE" are page-aligned,
		// but we still check so that, if we ever use a different bootloader that
		// doesn't have this behavior, it won't be a problem.
		uint16_t addr_adjustment = (PAGE_SIZE - (current_entry.address % PAGE_SIZE)) % PAGE_SIZE;
		uint16_t size_adjustment = current_entry.size % PAGE_SIZE;

		current_entry.address += addr_adjustment;
		current_entry.size    -= addr_adjustment;
		current_entry.size    -= size_adjustment;

		if(addr_adjustment != 0 || size_adjustment != 0) {
			klogf(LogLevel::Warning, "Adjusted range to: { address: %xl, size: %xl, type: %s }"
			    , current_entry.address
			    , current_entry.size
			    , memory_map_type_as_string(current_entry.type));
		}

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
			if(!contiguous_physical_ranges.is_empty()
			   && contiguous_physical_ranges.last().start.offset(contiguous_physical_ranges.last().size) == page_addr) {
				// If out list isn't empty, and the page we're looking at is the next sequential
				// page after the last one we looked at, then we add a page to the size of the
				// current contiguous range.
				contiguous_physical_ranges.last().size += PAGE_SIZE;
			} else {
				contiguous_physical_ranges.append(ContiguousPhysicalRange {
					.start = page_addr,
					.size  = PAGE_SIZE
				});
			}

		}

	}

	for(size_t i = 0; i < contiguous_physical_ranges.count; i++) {
		klogf(LogLevel::Info, "[Usable] ContiguousPhysicalRange: { start: %xl, size: %xl }"
		    , contiguous_physical_ranges[i].start.addr
		    , contiguous_physical_ranges[i].size);
	}

	locate_reserved_physical_ranges();

	for(size_t i = 0; i < contiguous_reserved_physical_ranges.count; i++) {
		klogf(LogLevel::Info, "[Reserved] ContiguousPhysicalRange: { start: %xl, size: %xl }"
		    , contiguous_reserved_physical_ranges[i].start.addr
		    , contiguous_reserved_physical_ranges[i].size);
	}

	initialize_pages();
}

void init_memory_management(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
}

}
