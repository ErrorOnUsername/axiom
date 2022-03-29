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

bool PhysicalMemoryRegion::is_addr_in_region(addr_t addr) const
{
	return ((addr >= start) && (addr < (start + size)));
}

bool PhysicalMemoryRegion::is_range_in_region(MemoryRange const& range)
{
	return (range.start >= start) && ((range.start + range.size) <= (start + size));
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

void PhysicalMemoryRegion::set_range_used(MemoryRange& range, bool used)
{
	ASSERT(range.is_page_aligned());

	physical_page_bitmap.set_range((range.start - start) / PAGE_SIZE, range.page_count(), used);
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

	MemoryRange range {
		.start = start + (index * PAGE_SIZE),
		.size  = page_count * PAGE_SIZE
	};

	set_range_used(range, true);
	return range;
}

void PhysicalMemoryRegion::free_physical_memory_range(MemoryRange& range)
{
	if(!is_addr_in_region(range.start))
		return;

	size_t page_count = range.size / PAGE_SIZE;
	size_t index      = (range.start - start) / PAGE_SIZE;

	ASSERT(physical_page_bitmap.verify_range_occupied(index, page_count));

	physical_page_bitmap.set_range(index, page_count, false);
	range.start = 0;
	range.size  = 0;
}

bool PhysicalMemoryRegion::is_range_used(MemoryRange const& range)
{
	ASSERT(range.is_page_aligned());
	ASSERT(is_range_in_region(range));

	size_t index = (range.start - start) / PAGE_SIZE;
	size_t count = range.page_count();
	bool thing = physical_page_bitmap.verify_range_occupied(index, count);

	return thing;
}

}
