#pragma once

#include <libs/ax/result.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/paging.hh>
#include <kernel/memory/region.hh>

namespace Kernel::Memory {


void init_memory_management(BootloaderMemoryMap&);

void switch_address_space(AddressSpace*);

AX::Result memory_map(AddressSpace* address_space, MemoryRange& virtual_range, AllocationFlags);
AX::Result memory_map_indentity(AddressSpace* address_space, MemoryRange& physical_range, AllocationFlags);

MemoryRange memory_allocate(AddressSpace* address_space, size_t size, AllocationFlags);
MemoryRange memory_allocate_page_identity(AddressSpace* address_space, AllocationFlags);

void memory_free(AddressSpace* address_space, MemoryRange& virtual_range);

}
