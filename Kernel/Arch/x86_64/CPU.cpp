#include <Kernel/Arch/CPU.h>

namespace Kernel {

void CPU::halt() { asm volatile("cli; hlt"); }

}
