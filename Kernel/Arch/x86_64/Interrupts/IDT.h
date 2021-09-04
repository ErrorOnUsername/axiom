#pragma once

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>

#define IDT_ENTRY_COUNT 256

#define IDT_INTERRUPT_GATE_TA 0x8e
#define IDT_TRAP_GATE_TA      0x8f

namespace Kernel {

struct IDTEntryDescriptor {
	uint16_t offset_0;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_and_attributes;
	uint16_t offset_1;
	uint32_t offset_2;
	uint32_t reserved;
} PACKED;

void set_idt_entry(uint8_t interrupt_vector, uint8_t type_and_attributes, void (*handler)());
void set_offset_of_idt_entry(IDTEntryDescriptor* descriptor, uint64_t offset);

extern "C" void* isr_table[];

void init_idt();

}
