#include "context.hh"

#include <kernel/arch/amd64/boot/tables.hh>
#include <kernel/system/scheduler/thread.hh>

namespace Kernel {

Context::Context()
	: userspace_stack_ptr(0)
	, kernel_stack_ptr(0)
	, registers()
{ }

void Context::init(uintptr_t ip, uintptr_t sp, uintptr_t ksp, ABIArgs args, ThreadPermissions perms)
{
	registers.rip = ip;
	registers.rflags = RFlags { .int_flag = 1 };

	// ABI stuff
	registers.rdi = args.arg_1;
	registers.rsi = args.arg_2;
	registers.rdx = args.arg_3;
	registers.rcx = args.arg_4;
	registers.r8  = args.arg_5;
	registers.r9  = args.arg_6;

	switch(perms) {
		case ThreadPermissions::KernelMode: {
			registers.rflags.iopl_flag = 0;
			registers.cs = (u64)GDTEntryOffset::KernelCodeSegment;
			registers.ss = (u64)GDTEntryOffset::KernelDataSegment;
			break;
		}
		case ThreadPermissions::UserMode: {
			registers.rflags.iopl_flag = 3;
			registers.cs = (u64)GDTEntryOffset::UserspaceCodeSegment | 3;
			registers.ss = (u64)GDTEntryOffset::UserspaceDataSegment | 3;
			break;
		}
	}

	registers.rsp = sp;
	kernel_stack_ptr = ksp;
}

}
