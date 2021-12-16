#pragma once

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Memory/BootloaderMemoryMap.h>

namespace Kernel {

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id);
const char* memory_map_type_as_string(uint32_t type);

BootloaderMemoryMapEntry memory_map_entry_from_stivale2_entry(stivale2_mmap_entry const&);

}
