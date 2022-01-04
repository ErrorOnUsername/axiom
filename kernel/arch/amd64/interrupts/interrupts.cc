#include <ax_util/types.hh>
#include <kernel/k_debug.hh>

namespace Kernel {

extern "C" [[noreturn]] void exception_handler(uint32_t vector);
void exception_handler(uint32_t vector)
{
	klogf(LogLevel::Error, "EXCEPTION TRIGGERED!! [%d] HANGING...", vector);
	asm volatile("cli; hlt");
	for(;;);
}

}
