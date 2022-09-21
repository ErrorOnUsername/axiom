#include "context.hh"

namespace Kernel {

void Context::init(uintptr_t ip, uintptr_t sp, uintptr_t ksp)
{
	registers.rip = ip;
	registers.rflags = 0;

	registers.rsp = sp;
	kernel_stack_ptr = ksp;
}

}
