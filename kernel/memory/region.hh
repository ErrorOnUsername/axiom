#pragma once

#include <ax_util/bitmap.hh>
#include <ax_util/lock.hh>
#include <ax_util/types.hh>

namespace Kernel::Memory {

struct MemoryRange {
	addr_t start = 0;
	size_t size  = 0;
};

struct PhysicalMemoryRegion {
	addr_t start = 0;
	size_t size  = 0;
	size_t allocated_pages = 0;

	AX::Bitmap physical_page_bitmap;

	size_t page_count() const;
	size_t free_pages() const;
	bool   is_addr_in_range(addr_t addr) const;

	void initialize_physical_memory_management();

	MemoryRange allocate_physical_pages(size_t page_count);
	void        free_physical_memory_range(MemoryRange&);
};

}
