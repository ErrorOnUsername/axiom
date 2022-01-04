#pragma once

#include <kernel/memory/bootloader_memory_map.hh>

namespace Kernel::Memory {

void init_memory_management(BootloaderMemoryMap&);

}
