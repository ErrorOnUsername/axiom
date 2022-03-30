#pragma once

#include <ax_util/types.hh>

#define REGISTER_STATE_SIZE (8 * 18)

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

	u64 interrupt_vector;
	u64 error_code;
};
