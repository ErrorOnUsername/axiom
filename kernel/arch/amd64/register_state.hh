#pragma once

#include <libs/ax/types.hh>

#define REGISTER_STATE_SIZE (8 * 20)

namespace Kernel {

struct RFlags {
	u64 cary          : 1  = 0;
	u64 reserved0     : 1  = 1; // Always 1
	u64 parity        : 1  = 0;
	u64 reserved1     : 1  = 0; // Always 0
	u64 aux_flag      : 1  = 0;
	u64 reserved2     : 1  = 0; // Always 0
	u64 zero_flag     : 1  = 0;
	u64 sign_flag     : 1  = 0;
	u64 trap_flag     : 1  = 0;
	u64 int_flag      : 1  = 1;
	u64 dir_flag      : 1  = 0;
	u64 overflow_flag : 1  = 0;
	u64 iopl_flag     : 2  = 0;
	u64 nested_flag   : 1  = 0;
	u64 reserved3     : 1  = 0; // Always 0
	u64 resume_flag   : 1  = 0;
	u64 v8086_mode    : 1  = 0;
	u64 align_check   : 1  = 0;
	u64 vint_flag     : 1  = 0;
	u64 vint_pending  : 1  = 0;
	u64 id_flag       : 1  = 0;
	u64 reserved4     : 42 = 0;
};

static_assert(sizeof(RFlags) == sizeof(u64), "RFlags needs to be 64-bits wide");

struct RegisterState {
	u64 rip;
	u64 rsp;
	u64 rbp;
	u64 rsi;
	u64 rdi;

	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;

	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;

	RFlags rflags;

	u64 cs;
	u64 ss;

	u64 interrupt_vector;
	u64 error_code;
};

struct ABIArgs {
	u64 arg_1;
	u64 arg_2;
	u64 arg_3;
	u64 arg_4;
	u64 arg_5;
	u64 arg_6;
};

}
