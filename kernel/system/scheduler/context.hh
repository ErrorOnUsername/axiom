#pragma once

// FIXME: This is a hack! Make it platform agnostic!
#include <kernel/arch/amd64/register_state.hh>
#include <kernel/memory/region.hh>

namespace Kernel {

struct Context {
	RegisterState registers;

	uintptr_t kernel_stack_pointer;
	Memory::MemoryRange kernel_stack;
};

}
