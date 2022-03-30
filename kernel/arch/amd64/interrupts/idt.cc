#include "idt.hh"

#include <ax_util/types.hh>
#include <kernel/arch/amd64/gdt/gdt.hh>
#include <kernel/arch/amd64/descriptor_table_pointer.hh>
#include <kernel/k_debug.hh>

namespace Kernel::IDT {

static DescriptorTablePointer idtr;
static IDTEntryDescriptor idt[IDT_ENTRY_COUNT];

void set_offset_of_idt_entry(IDTEntryDescriptor* descriptor, u64 offset)
{
	descriptor->offset_0 = (u16)((offset & 0x000000000000ffff) >> 0);
	descriptor->offset_1 = (u16)((offset & 0x00000000ffff0000) >> 16);
	descriptor->offset_2 = (u32)((offset & 0xffffffff00000000) >> 32);
}

void set_idt_entry(u8 interrupt_vector, u8 type_and_attributes, void* handler)
{
	IDTEntryDescriptor* entry = &idt[interrupt_vector];

	set_offset_of_idt_entry(entry, (addr_t)handler);
	entry->selector = (u16)GDT::GDTEntryOffset::KernelCodeSegment;
	entry->type_and_attributes = type_and_attributes;
	entry->ist = 0; //FIXME: Actually use this like we're supposed to
	entry->reserved = 0;
}

void init_idt()
{
	idtr.offset = (addr_t)&idt[0];
	idtr.size = (IDT_ENTRY_COUNT * sizeof(IDTEntryDescriptor)) - 1;

	for(u8 vec = 0; vec < 32; vec++) {
		set_idt_entry(vec, IDT_INTERRUPT_GATE_TA, isr_table[vec]);
	}

	asm volatile("lidt %0" :: "memory"(idtr));
	asm volatile("sti");

	klogf(LogLevel::Info, "loaded IDT with size: %x offset: %xl", idtr.size, idtr.offset);
}

}
