#pragma once

#include <ax_util/helpers.hh>
#include <ax_util/types.hh>
#include <kernel/arch/amd64/descriptor_table_pointer.hh>

#define IDT_ENTRY_COUNT 256

#define IDT_INTERRUPT_GATE_TA 0x8e
#define IDT_TRAP_GATE_TA      0x8f

namespace Kernel::IDT {

struct IDTEntryDescriptor {
	u16 offset_0;
	u16 selector;
	u8  ist;
	u8  type_and_attributes;
	u16 offset_1;
	u32 offset_2;
	u32 reserved;
} PACKED;

void set_idt_entry(u8 interrupt_vector, u8 type_and_attributes, void* handler);
void set_offset_of_idt_entry(IDTEntryDescriptor* descriptor, u64 offset);

extern "C" void* isr_table[];

void init();

}
