#pragma once

#include <kernel/memory/bootloader_memory_map.h>

namespace Kernel::Memory {

void init_memory_management(BootloaderMemoryMap&);

}
