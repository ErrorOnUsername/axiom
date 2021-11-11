#include <AXUtil/Types.h>
#include <Kernel/k_stdio.h>

namespace Kernel {

extern "C" [[noreturn]] void exception_handler(uint32_t vector);
void exception_handler(uint32_t vector)
{
	k_printf("EXCEPTION TRIGGERED!! [%d] HANGING...\n", vector);
	asm volatile("cli; hlt");
}

}
