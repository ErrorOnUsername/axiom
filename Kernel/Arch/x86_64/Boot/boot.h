#pragma once

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>

namespace Kernel {

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id);
const char* memmap_type_as_string(uint32_t type);

}
