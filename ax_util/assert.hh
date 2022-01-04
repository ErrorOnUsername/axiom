#pragma once

#include <ax_util/helpers.hh>
#include <Kernel/Arch/CPU.h>
// FIXME: Think of a better way of doing this
#include <Kernel/k_debug.h>

#define __HANDLE_ASSERT_FAILED_WITH_MESSAGE__(condition, msg...)                              \
	Kernel::k_printf("[" __FILE__ ":" __STRINGIFY__(__LINE__) "] Assertion Failed: " msg); \
	Kernel::CPU::halt();
#define __HANDLE_ASSERT_FAILED__(condition)                                                                                  \
	Kernel::k_printf("[" __FILE__ ":" __STRINGIFY__(__LINE__) "] Assertion Failed: " #condition ". Halting execution...\n"); \
	asm volatile("cli;hlt");

#define ASSERT(condition) do { if(!(condition)) { __HANDLE_ASSERT_FAILED__(condition) } } while(0)
#define ASSERT_MSG(condition, msg...) do { if(!(condition)) { __HANDLE_ASSERT_FAILED_WITH_MESSAGE__(condition, msg) } } while(0)
#define ASSERT_NOT_REACHED() do { __HANDLE_ASSERT_FAILED__(ASSERT_NOT_REACHED) } while(0)
