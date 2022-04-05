#include "scheduler.hh"

#include <ax_util/lock.hh>
#include <ax_util/queue.hh>
#include <kernel/k_debug.hh>

namespace Kernel::Scheduler {

static AX::Lock scheduler_lock;

static AX::Queue<int> kernel_tasks;
static AX::Queue<int> user_tasks;

AX::Result init()
{
	kernel_tasks.ensure_capacity(8);
	user_tasks.ensure_capacity(16);

	return AX::Result::Success;
}

extern "C" void on_task_switch_timer()
{
	// 1. Save the current context
	// 2. Pick the next task to run
	// 3. Load the next context
	// 4. Swap to that task's address space
}

}
