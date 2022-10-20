#pragma once

#include <kernel/arch/interrupts.hh>

namespace Kernel::PIT {

void init(IRQHandler);

}
