#include "thread.hh"

namespace Kernel {

Thread::Thread(Process* proccess, AXCallback thread_proc)
	: owning_process(proccess)
{
	(void)thread_proc;
}

void Thread::save_context() { }

void Thread::load_context() { }

}
