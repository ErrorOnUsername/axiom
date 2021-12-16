#include "GDT.h"

#include <AXUtil/Types.h>
#include <Kernel/k_debug.h>

namespace Kernel::GDT {

GDT custom_gdt = {
	{0, 0, 0, 0x00, 0x00, 0}, /* NULL */

	/* Kernel Segments */
	{0, 0, 0, 0x9a, 0xa0, 0}, /* CODE */
	{0, 0, 0, 0x92, 0xa0, 0}, /* DATA */

	/* Userspace Segments */
	{0, 0, 0, 0xfa, 0xa0, 0}, /* CODE */
	{0, 0, 0, 0xf2, 0xa0, 0}, /* DATA */
};

void init_gdt()
{
	DescriptorTablePointer descriptor { };
	descriptor.size = sizeof(custom_gdt) - 1;
	descriptor.offset = (uint64_t)&custom_gdt;
	load_gdt(&descriptor);
	klogf(LogLevel::Info, "loaded GDT with size: %x offset: %xl", descriptor.size, descriptor.offset);
}

}
