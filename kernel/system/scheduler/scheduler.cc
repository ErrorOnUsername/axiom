#include "scheduler.hh"

#include <ax_util/lock.hh>
#include <ax_util/queue.hh>
#include <kernel/arch/processor.hh>
#include <kernel/system/scheduler/thread.hh>
#include <kernel/memory/paging.hh>
#include <kernel/time/pit.hh>
#include <kernel/k_debug.hh>

namespace Kernel {

static void on_task_switch_timer();

namespace Scheduler {

static AX::Lock scheduler_lock;

static AX::Queue<int> kernel_tasks;
static AX::Queue<int> user_tasks;

AX::Result init()
{
	kernel_tasks.ensure_capacity(8);
	user_tasks.ensure_capacity(16);

	PIT::init((void*)&on_task_switch_timer);

	return AX::Result::Success;
}

void pick_next()
{
	Thread* current_thread = Proccessor::current_thread();
	(void)current_thread;
}

}

static void on_task_switch_timer()
{
	Proccessor::current_thread()->save_context();
	Scheduler::pick_next();
	Thread* new_thread = Proccessor::current_thread();
	if(!new_thread)
		return;
	new_thread->load_context();
	Memory::switch_address_space(new_thread->owning_process->addr_space);
}

}
