#include <Kernel/k_stdio.h>

namespace Kernel {

extern "C" [[noreturn]] void exception_handler();
void exception_handler()
{
	k_printf("EXCEPTION TRIGGERED! HANGING...\n");
	asm volatile("cli; hlt");
}

}
