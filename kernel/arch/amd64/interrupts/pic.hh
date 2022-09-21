#pragma once

#include <libs/ax/types.hh>

namespace Kernel::PIC {

void init();
void disable();

void end_of_interrupt(u8 irq_number);

}
