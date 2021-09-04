#include "IDT.h"

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>
#include <Kernel/k_stdio.h>

namespace Kernel {

static DescriptorTablePointer idtr;
static IDTEntryDescriptor idt[IDT_ENTRY_COUNT];

void set_offset_of_idt_entry(IDTEntryDescriptor* descriptor, uint64_t offset)
{
	descriptor->offset_0 = (uint16_t)((offset & 0x000000000000ffff) >> 0);
	descriptor->offset_1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
	descriptor->offset_2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

void set_idt_entry(uint8_t interrupt_vector, uint8_t type_and_attributes, void* handler)
{
	IDTEntryDescriptor* entry = &idt[interrupt_vector];

	set_offset_of_idt_entry(entry, (uint64_t)handler);
	entry->selector = (uint16_t)GDTEntryOffset::KernelCodeSegment;
	entry->type_and_attributes = type_and_attributes;
	entry->ist = 0; //FIXME: Actually use this like we're supposed to
	entry->reserved = 0;
}

void init_idt()
{
	idtr.offset = (uint64_t)&idt[0];
	idtr.size = (IDT_ENTRY_COUNT * sizeof(IDTEntryDescriptor)) - 1;

	for(uint8_t vec = 0; vec < 32; vec++) {
		set_idt_entry(vec, IDT_INTERRUPT_GATE_TA, isr_table[vec]);
	}

	asm volatile("lidt %0" :: "memory"(idtr));
	asm volatile("sti");
	k_printf("[IDT] loaded with size: %x offset %xl\n", (uint32_t)idtr.size, idtr.offset);
}

}
