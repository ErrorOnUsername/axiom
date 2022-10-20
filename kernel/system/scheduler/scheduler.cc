#include "scheduler.hh"

#include <libs/ax/lock.hh>
#include <libs/ax/queue.hh>
#include <kernel/arch/processor.hh>
#include <kernel/arch/interrupts.hh>
#include <kernel/memory/paging.hh>
#include <kernel/time/pit.hh>
#include <kernel/k_debug.hh>

namespace Kernel {

static void on_task_switch_timer(RegisterState&);

namespace Scheduler {

static AX::Lock scheduler_lock;

static AX::Queue<Thread*> kernel_tasks;
static AX::Queue<Thread*> user_tasks;

void init()
{
	kernel_tasks.ensure_capacity(8);
	user_tasks.ensure_capacity(16);

	PIT::init(&on_task_switch_timer);
}

void start(Thread* thread, uintptr_t ip, uintptr_t sp)
{
	AX::ScopeLocker locker(scheduler_lock);

	ASSERT(!thread->started);

	thread->context->init(ip, sp, 0 /* ksp */, ABIArgs{}, thread->permissions);
	thread->started = true;

	enqueue_thread(thread);
}

void pick_next()
{
	Thread* current_thread = Processor::current_thread();
	(void)current_thread;
}

void enqueue_thread(Thread*) { }

}

static void on_task_switch_timer(RegisterState& regs)
{
	ScopeInterruptDisabler disabler;

	Processor::current_thread()->save_context(regs);
	Scheduler::pick_next();
	Thread* new_thread = Processor::current_thread();
	if(!new_thread)
		return;
	new_thread->load_context_to(regs);
	Memory::switch_address_space(new_thread->owning_process->addr_space);
}

}
