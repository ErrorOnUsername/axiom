#pragma once

#ifdef AXIOM_ARCH_AMD64

#include <kernel/arch/amd64/interrupts/interrupts.hh>

#else
#error "OH NO! We don't support that architecture!"
#endif
