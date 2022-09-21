#pragma once

#include <kernel/arch/register_state.hh>
#include <kernel/memory/region.hh>

namespace Kernel {

struct Context {
	uintptr_t userspace_stack_ptr;
	uintptr_t kernel_stack_ptr;

	RegisterState registers;

	Memory::MemoryRange userspace_stack;
	Memory::MemoryRange kernel_stack;

	void init(uintptr_t ip, uintptr_t sp, uintptr_t ksp);
};

}
