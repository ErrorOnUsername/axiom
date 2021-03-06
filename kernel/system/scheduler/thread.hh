#pragma once

#include <ax_util/types.hh>
#include <kernel/system/scheduler/context.hh>
#include <kernel/system/scheduler/proccess.hh>

namespace Kernel {

struct Thread {
	Process* owning_process;
	Context* context;
	u64      cpu;
	int      tid;
	int      pid;

	Thread(Process*, AXCallback thread_proc);

	void save_context();
	void load_context();
};

};
