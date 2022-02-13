#include "asm_utils.hh"

namespace Kernel {

void load_cr3(void* addr_space)
{
	asm volatile("mov %%rax, %%cr3" :: "a"(addr_space)
	           : "memory");
}

}
