#include <kernel/arch/CPU.h>

namespace Kernel {

void CPU::halt() { asm volatile("cli; hlt"); }

void CPU::disable_interrupts() { asm volatile("cli"); }

void CPU::enable_interrupts() { asm volatile("sti"); }

}
