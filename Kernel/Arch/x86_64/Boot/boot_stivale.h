#pragma once

#include <AXUtil/Types.h>
#include <kernel/arch/x86_64/boot/stivale2.h>
#include <kernel/memory/bootloader_memory_map.h>

namespace Kernel {

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id);

Memory::BootloaderMemoryMapEntry memory_map_entry_from_stivale2_entry(stivale2_mmap_entry const&);

}
