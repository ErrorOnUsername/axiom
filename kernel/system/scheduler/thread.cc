#include "thread.hh"

namespace Kernel {

Thread::Thread(Process* proccess, int tid, AXCallback thread_proc, ABIArgs abi_args, ThreadPermissions perms)
	: owning_process(proccess)
	, context(nullptr)
	, cpu(0)
	, tid(tid)
	, pid(proccess->pid)
	, in_syscall(false)
	, blocked(false)
	, stopped(true)
	, started(false)
	, permissions(perms)
{
	// TODO: This is where we left off:
	//     1) allocate the stacks
	//     2) map them where they need to go
	//     3) make Proccess class that simply
	//        creates these and queues them into
	//        the scheduler
	userspace_stack;
	kernel_stack;

	context = new Context();
	context->init((uintptr_t)((void*)thread_proc),
	              userspace_stack.start,
	              kernel_stack.start,
	              abi_args,
	              permissions);
}

Thread::~Thread()
{
	delete context;
}

void Thread::save_context(RegisterState const& regs)
{
	context->registers = regs;
}

void Thread::load_context_to(RegisterState& regs)
{
	regs = context->registers;
}

}
