#pragma once

#include <libs/ax/types.hh>
#include <kernel/arch/amd64/boot/stivale2.h>
#include <kernel/memory/bootloader_memory_map.hh>

namespace Kernel {

void* get_stivale2_tag(stivale2_struct* stivale2_struct, u64 id);

Memory::BootloaderMemoryMapEntry memory_map_entry_from_stivale2_entry(stivale2_mmap_entry const&);

}
