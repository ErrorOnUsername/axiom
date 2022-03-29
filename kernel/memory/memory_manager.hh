#pragma once

#include <ax_util/result.hh>
#include <kernel/arch/amd64/paging.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/region.hh>

namespace Kernel::Memory {

void init_memory_management(BootloaderMemoryMap&);

AX::Result memory_map(PML4T* address_space, MemoryRange& virtual_range, AllocationFlags);
AX::Result memory_map_indentity(PML4T* address_space, MemoryRange& physical_range, AllocationFlags);

MemoryRange memory_allocate(PML4T* address_space, size_t size, AllocationFlags);
MemoryRange memory_allocate_page_identity(PML4T* address_space, AllocationFlags);

void memory_free(PML4T* address_space, MemoryRange& virtual_range);

}
