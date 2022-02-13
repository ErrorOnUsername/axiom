#include "memory_manager.hh"

#include <ax_util/bitmap.hh>
#include <ax_util/helpers.hh>
#include <ax_util/list.hh>
#include <ax_util/lock.hh>
#include <kernel/arch/amd64/paging.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/region.hh>
#include <kernel/k_debug.hh>

//
// TODO:
//   * Make a virtual memory manager.
//

namespace Kernel::Memory {

static AX::List<PhysicalMemoryRegion> usable_contiguous_ranges;
static AX::Lock                       allocator_lock;

static void parse_memory_map(BootloaderMemoryMap&);
static void initialize_physical_memory_management();

void init_memory_management(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
	initialize_physical_memory_management();
	init_virtual_memory();
}

MemoryRange allocate_physical_pages(size_t page_count)
{
	AX::ScopeLocker locker(allocator_lock);

	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.free_pages() >= page_count)
			return region.allocate_physical_pages(page_count);
	}

	ASSERT_MSG(false, "OOM! Ran out of physical memory!");

	return MemoryRange { 0, 0 };
}

MemoryRange allocate_physical_page_aligned_region_in_bytes(size_t bytes)
{
	return allocate_physical_pages(ALIGN_UP(bytes, PAGE_SIZE) / PAGE_SIZE);
}

void free_physical_memory_range(MemoryRange& range)
{
	if(range.start < 0x1000)
		return;

	AX::ScopeLocker locker(allocator_lock);

	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.start + region.size > range.start)
			region.free_physical_memory_range(range);
	}
}

static void locate_reserved_physical_ranges(BootloaderMemoryMap const& memory_map)
{
	BootloaderMemoryMapEntry current_entry;

	addr_t reserved_range_start_addr = 0x0000000000000000;
	bool   is_last_range_reserved    = false;

	for(size_t i = 0; i < memory_map.length; i++) {
		current_entry = memory_map.entries[i];

		if(current_entry.type != MemoryRegionType::Usable
		   && current_entry.type != MemoryRegionType::BootloaderReclaimable) {
			if(!is_last_range_reserved)
				reserved_range_start_addr = current_entry.address;

			is_last_range_reserved = true;
		} else {
			if(is_last_range_reserved)
				klogf(LogLevel::Info, "[Reserved] PhysicalMemoryRegion: { %xl - %xl }"
				    , reserved_range_start_addr
				    , current_entry.address - 1);
			is_last_range_reserved = false;
		}
	}

	if(is_last_range_reserved) {
		klogf(LogLevel::Info, "[Reserved] PhysicalMemoryRegion: { %xl - %xl }"
		    , reserved_range_start_addr
		    , current_entry.address - 1);
	}
}

static void parse_memory_map(BootloaderMemoryMap& memory_map)
{
	for(uint64_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry& current_entry = memory_map.entries[i];

		klogf(LogLevel::Info, "PhysicalMemoryRegion: { address: %xl, length: %xl, type: %s }"
		    , current_entry.address
		    , current_entry.size
		    , memory_map_type_as_string(current_entry.type));

		if(current_entry.type != MemoryRegionType::Usable
		   && current_entry.type != MemoryRegionType::BootloaderReclaimable)
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
			klogf(LogLevel::Warning, "Adjusted PhysicalMemoryRegion to: { address: %xl, size: %xl, type: %s }"
			    , current_entry.address
			    , current_entry.size
			    , memory_map_type_as_string(current_entry.type));
		}

		if(current_entry.size < PAGE_SIZE) {
			klogf(LogLevel::Warning, "The bootloader reported a PhysicalMemoryRegion [%xl - %xl] with a size less than %u (one page)! Skipping it..."
			    , current_entry.address
			    , current_entry.address + (current_entry.size - 1)
			    , PAGE_SIZE);
			continue;
		}

		// Walk the pages in each Usable section to find all contiguous regions
		for(uint64_t page_addr = current_entry.address;
		             page_addr < (current_entry.address + current_entry.size);
		             page_addr += PAGE_SIZE) {
			if(!usable_contiguous_ranges.is_empty()
			   && usable_contiguous_ranges.last().start + usable_contiguous_ranges.last().size == page_addr) {
				// If out list isn't empty, and the page we're looking at is the next sequential
				// page after the last one we looked at, then we add a page to the size of the
				// current contiguous range.
				usable_contiguous_ranges.last().size += PAGE_SIZE;
			} else {
				usable_contiguous_ranges.append(PhysicalMemoryRegion {
					.start = page_addr,
					.size  = PAGE_SIZE
				});
			}
		}
	}

	for(size_t j = 0; j < usable_contiguous_ranges.count; j++) {
		klogf(LogLevel::Info, "[Usable] PhysicalMemoryRegion: { start: %xl, size: %xl }"
			, usable_contiguous_ranges[j].start
			, usable_contiguous_ranges[j].size);
	}

	locate_reserved_physical_ranges(memory_map);
}

static void initialize_physical_memory_management()
{
	for(size_t i = 0; i < usable_contiguous_ranges.count; i++)
		usable_contiguous_ranges[i].initialize_physical_memory_management();
}

}
