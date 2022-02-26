#pragma once

#include <ax_util/result.hh>
#include <kernel/arch/amd64/paging.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/region.hh>

namespace Kernel::Memory {

void init_physical_memory_manager(BootloaderMemoryMap& memory_map);

void physical_set_range_used(MemoryRange&);
void physical_set_range_free(MemoryRange&);

MemoryRange physical_allocate_pages(size_t page_count);
MemoryRange physical_allocate_page_aligned_range_in_bytes(size_t bytes);

void physical_free_memory_range(MemoryRange&);

}
