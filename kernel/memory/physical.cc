#include "physical.hh"

#include <libs/ax/bitmap.hh>
#include <libs/ax/list.hh>
#include <kernel/k_debug.hh>
#include <kernel/panic.hh>
#include <kernel/memory/region.hh>
#include <kernel/memory/bootloader_memory_map.hh>

namespace Kernel::Memory {

static size_t highest_page = 0;

static AX::List<PhysicalMemoryRegion> usable_contiguous_ranges;
static AX::Lock                       allocator_lock;

static void parse_memory_map(BootloaderMemoryMap const& memory_map);
static void locate_reserved_physical_ranges(BootloaderMemoryMap const& memory_map);
static void initialize_physical_memory_management();

void init_physical_memory_manager(BootloaderMemoryMap& memory_map)
{
	parse_memory_map(memory_map);
	initialize_physical_memory_management();
}

size_t physical_highest_page()
{
	return highest_page;
}

void physical_set_range_used(MemoryRange& physical_range)
{
	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.is_range_in_region(physical_range)) {
			region.set_range_used(physical_range, true);
		}
	}
}

void physical_set_range_free(MemoryRange& physical_range)
{
	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.is_range_in_region(physical_range)) {
			region.set_range_used(physical_range, false);
		}
	}
}

bool physical_is_range_used(MemoryRange& physical_range)
{
	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(physical_range.start >= region.start && (physical_range.start + physical_range.size) <= (region.start + region.size)) {
			return region.is_range_used(physical_range);
		}
	}
	return true;
}

MemoryRange physical_allocate_pages(size_t page_count)
{
	AX::ScopeLocker locker(allocator_lock);

	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.free_pages() >= page_count)
			return region.allocate_physical_pages(page_count);
	}

	panic("OOM! Ran out of physical memory!");
	return MemoryRange { 0, 0 };
}

MemoryRange physical_allocate_page_aligned_range_in_bytes(size_t bytes)
{
	return physical_allocate_pages(ALIGN_UP(bytes, PAGE_SIZE) / PAGE_SIZE);
}

void physical_free_memory_range(MemoryRange& physical_range)
{
	if(physical_range.start < 0x1000)
		return;

	AX::ScopeLocker locker(allocator_lock);

	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		PhysicalMemoryRegion& region = usable_contiguous_ranges[i];
		if(region.start + region.size > physical_range.start)
			region.free_physical_memory_range(physical_range);
	}
}

static void parse_memory_map(BootloaderMemoryMap const& memory_map)
{
	for(u64 i = 0; i < memory_map.length; i++) {
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
		u16 addr_adjustment = (PAGE_SIZE - (current_entry.address % PAGE_SIZE)) % PAGE_SIZE;
		u16 size_adjustment = current_entry.size % PAGE_SIZE;

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

		size_t page_index = ((current_entry.address + current_entry.size) - PAGE_SIZE) / PAGE_SIZE;
		if(page_index > highest_page)
			highest_page = page_index;

		// Walk the pages in each Usable section to find all contiguous regions
		for(u64 page_addr = current_entry.address;
		        page_addr < (current_entry.address + current_entry.size);
		        page_addr += PAGE_SIZE)
		{
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

	for(size_t i = 0; i < usable_contiguous_ranges.count; i++) {
		klogf(LogLevel::Info, "[Usable] PhysicalMemoryRegion: { start: %xl, size: %xl }"
			, usable_contiguous_ranges[i].start
			, usable_contiguous_ranges[i].size);
	}

	locate_reserved_physical_ranges(memory_map);
}

static void locate_reserved_physical_ranges(BootloaderMemoryMap const& memory_map)
{
	BootloaderMemoryMapEntry current_entry;

	addr_t reserved_range_start_addr = 0;
	bool   is_last_range_reserved    = false;

	for(size_t i = 0; i < memory_map.length; i++) {
		current_entry = memory_map.entries[i];

		if(current_entry.type != MemoryRegionType::Usable
		   && current_entry.type != MemoryRegionType::BootloaderReclaimable)
		{
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

static void initialize_physical_memory_management()
{
	for(size_t i = 0; i < usable_contiguous_ranges.count; i++)
		usable_contiguous_ranges[i].initialize_physical_memory_management();
}

}
