#include <AXUtil/Types.h>
#include <Kernel/k_debug.h>

namespace Kernel {

extern "C" [[noreturn]] void exception_handler(uint32_t vector);
void exception_handler(uint32_t vector)
{
	klogf(LogModeError, "EXCEPTION TRIGGERED!! [%d] HANGING...", vector);
	asm volatile("cli; hlt");
}

}
