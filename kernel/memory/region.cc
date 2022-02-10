#include "region.hh"

#include <ax_util/assert.hh>
#include <ax_util/helpers.hh>
#include <ax_util/lock.hh>
#include <kernel/k_debug.hh>

namespace Kernel::Memory {

size_t PhysicalMemoryRegion::page_count() const
{
	ASSERT(size % PAGE_SIZE == 0);
	return size / PAGE_SIZE;
}

size_t PhysicalMemoryRegion::free_pages() const
{
	return page_count() - allocated_pages;
}

bool PhysicalMemoryRegion::is_addr_in_range(addr_t addr) const
{
	return ((addr >= start) && (addr < (start + size)));
}

void PhysicalMemoryRegion::initialize_physical_memory_management()
{
	physical_page_bitmap.init(page_count());
	physical_page_bitmap.fill(0x00);

	klogf(LogLevel::Info, "[%xl - %xl]: %us pages (%us KiB)"
	                    , start
	                    , start + (size - 1)
	                    , page_count()
	                    , size / KiB);
}

MemoryRange PhysicalMemoryRegion::allocate_physical_pages(size_t page_count)
{
	int64_t index = physical_page_bitmap.find_first_fit(page_count);
	if(index == -1) {
		klogf(LogLevel::Warning, "MemoryRegion: { start: %xl, size: %xl } could not allocate %us additional pages!"
		    , start
		    , size
		    , page_count);

		// FIXME: Do something more intelligent here
		ASSERT(false);
	}

	physical_page_bitmap.set_range(index, page_count, true);

	MemoryRange region {
		.start = start + (index * PAGE_SIZE),
		.size  = page_count * PAGE_SIZE
	};

	return region;
}

void PhysicalMemoryRegion::free_physical_memory_range(MemoryRange& range)
{
	if(!is_addr_in_range(range.start))
		return;

	size_t page_count = range.size / PAGE_SIZE;
	size_t index      = (range.start - start) / PAGE_SIZE;

	ASSERT(physical_page_bitmap.verify_range_occupied(index, page_count));

	physical_page_bitmap.set_range(index, page_count, false);
	range.start = 0;
	range.size  = 0;
}

}
