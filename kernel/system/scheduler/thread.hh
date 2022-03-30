#pragma once

#include <kernel/system/scheduler/proccess.hh>

namespace Kernel {

struct Thread {
	Process* owning_process;
	u64      cpu;
	int      tid;
	int      pid;

	Thread(Process*, void (*thread_proc)());
};

};
