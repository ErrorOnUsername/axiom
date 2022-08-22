#pragma once

#include <libs/ax/bitmap.hh>
#include <libs/ax/helpers.hh>
#include <libs/ax/lock.hh>
#include <libs/ax/types.hh>

namespace Kernel::Memory {

struct MemoryRange {
	addr_t start = 0;
	size_t size  = 0;

	inline size_t page_count() const { return ALIGN_UP(size, PAGE_SIZE) / PAGE_SIZE; }
	inline bool   is_page_aligned() const { return ((start % PAGE_SIZE) == 0) && ((size % PAGE_SIZE) == 0); }
};

struct PhysicalMemoryRegion {
	addr_t start = 0;
	size_t size  = 0;
	size_t allocated_pages = 0;

	AX::Bitmap physical_page_bitmap;

	size_t page_count() const;
	size_t free_pages() const;
	bool   is_addr_in_region(addr_t addr) const;
	bool   is_range_in_region(MemoryRange const&);

	void initialize_physical_memory_management();

	void set_range_used(MemoryRange&, bool used);

	MemoryRange allocate_physical_pages(size_t page_count);
	void        free_physical_memory_range(MemoryRange&);

	bool is_range_used(MemoryRange const&);
};

}
