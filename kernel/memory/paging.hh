#pragma once

#ifdef AXIOM_ARCH_AMD64

#include <kernel/arch/amd64/paging.hh>

namespace Kernel::Memory { using AddressSpace = PML4T; }

#else
#error "OH NO! We don't support that architecture!"
#endif
