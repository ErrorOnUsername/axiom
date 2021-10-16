#pragma once

#include <AXUtil/Helpers.h>
// FIXME: Think of a better way of doing this
#include <Kernel/k_stdio.h>

#define __HANDLE_ASSERT_FAILED__(condition)                                                                                  \
	Kernel::k_printf("[" __FILE__ ":" __STRINGIFY__(__LINE__) "] Assertion Failed: " #condition ". Halting execution...\n"); \
	asm volatile("cli;hlt");

#define ASSERT(condition) do { if(!(condition)) { __HANDLE_ASSERT_FAILED__(condition) } } while(0)
#define ASSERT_NOT_REACHED() do { __HANDLE_ASSERT_FAILED__(ASSERT_NOT_REACHED) } while(0)
