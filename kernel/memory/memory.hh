#pragma once

#include <ax_util/types.hh>

namespace Kernel::Memory {

#define KERNEL_VIRTUAL_START 0xffffffff80000000
#define IO_VIRTUAL_START 0xffff800000000000

inline addr_t kernel_to_physical(addr_t kernel_addr) { return kernel_addr - KERNEL_VIRTUAL_START; }

inline addr_t io_to_physical(addr_t kernel_addr) { return kernel_addr - IO_VIRTUAL_START; }

enum AllocationFlags : uint8_t {
	MEMORY_NONE  = 0b0000,
	CLEAR_MEMORY = 0b0001,
	USER_MEMORY  = 0b0010
};

}
