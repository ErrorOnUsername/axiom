#pragma once

namespace Kernel {

extern "C" void* isr_table[];

void init_descriptor_tables();

}
