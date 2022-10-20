#pragma once

#include <kernel/arch/register_state.hh>
#include <kernel/memory/region.hh>

namespace Kernel {

enum class ThreadPermissions;

struct Context {
	uintptr_t userspace_stack_ptr;
	uintptr_t kernel_stack_ptr;

	RegisterState registers;

	Context();

	void init(uintptr_t ip, uintptr_t sp, uintptr_t ksp, ABIArgs, ThreadPermissions);
};

}
