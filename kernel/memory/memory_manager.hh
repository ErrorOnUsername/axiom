#pragma once

#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/region.hh>

namespace Kernel::Memory {

void init_memory_management(BootloaderMemoryMap&);

MemoryRange allocate_physical_pages(size_t page_count);
MemoryRange allocate_physical_page_aligned_region_in_bytes(size_t bytes);
void        free_physical_memory_range(MemoryRange&);

}
