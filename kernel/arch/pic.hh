#pragma once

#include <ax_util/types.hh>

namespace Kernel::PIC {

void init();
void disable();

void enable_irq();
void disable_irq();

void end_of_interrupt(u8 irq_number);

}
