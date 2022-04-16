#pragma once
#ifdef AXIOM_ARCH_AMD64
#include <kernel/arch/amd64/paging.hh>

namespace Kernel::Memory {

using AddressSpace = PML4T;

void switch_address_space(AddressSpace* address_space);

}
#else
#error "We dont support this architecture!"
#endif
