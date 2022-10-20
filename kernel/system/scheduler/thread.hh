#pragma once

#include <libs/ax/types.hh>
#include <kernel/arch/register_state.hh>
#include <kernel/system/scheduler/context.hh>
#include <kernel/system/scheduler/proccess.hh>

namespace Kernel {

enum class ThreadPermissions {
	KernelMode,
	UserMode,
};

struct Thread {
	Process* owning_process;
	Context* context;

	u64 cpu;
	int tid;
	int pid;

	bool in_syscall;
	bool blocked;
	bool stopped;
	bool started;

	ThreadPermissions permissions;

	Memory::MemoryRange userspace_stack;
	Memory::MemoryRange kernel_stack;

	Thread(Process*, int tid, AXCallback thread_proc, ABIArgs, ThreadPermissions);
	~Thread();

	void save_context(RegisterState const&);
	void load_context_to(RegisterState&);
};

};
