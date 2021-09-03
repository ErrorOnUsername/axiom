#include "interrupts.h"

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/Interrupts/IDT.h>
#include <Kernel/Arch/x86_64/RegisterState.h>
#include <Kernel/k_stdio.h>

#define EXCEPTION_HANDLER_PANIC(vec, msg) \
	extern "C" void exception_##vec();    \
	void exception_##vec()                \
	{                                     \
		k_printf("%s\n", msg);            \
	}

namespace Kernel {

EXCEPTION_HANDLER_PANIC(0, "Divide By Zero");
EXCEPTION_HANDLER_PANIC(1, "Debug");
EXCEPTION_HANDLER_PANIC(2, "Unknown (NMI)");
EXCEPTION_HANDLER_PANIC(3, "Breakpoint");
EXCEPTION_HANDLER_PANIC(4, "Overflow");
EXCEPTION_HANDLER_PANIC(5, "Bound Range Exceeded");
EXCEPTION_HANDLER_PANIC(6, "Invalid Opcode");
EXCEPTION_HANDLER_PANIC(7, "Device Not Available");
EXCEPTION_HANDLER_PANIC(8, "Double Fault");
EXCEPTION_HANDLER_PANIC(9, "Coprocessor Segment Overrun [D]");
EXCEPTION_HANDLER_PANIC(10, "Invalid TSS");
EXCEPTION_HANDLER_PANIC(11, "Segment Not Present");
EXCEPTION_HANDLER_PANIC(12, "Stack Segment Fault");
EXCEPTION_HANDLER_PANIC(13, "General Protection Fault");
EXCEPTION_HANDLER_PANIC(14, "Page Fault");
EXCEPTION_HANDLER_PANIC(15, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(16, "x87 Floating Point Exception");
EXCEPTION_HANDLER_PANIC(17, "Alignment Check");
EXCEPTION_HANDLER_PANIC(18, "Machine Check");
EXCEPTION_HANDLER_PANIC(19, "SIMD Floating Point Exception");
EXCEPTION_HANDLER_PANIC(20, "Virtualization Exception");
EXCEPTION_HANDLER_PANIC(21, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(22, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(23, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(24, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(25, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(26, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(27, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(28, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(29, "Unknown (R)");
EXCEPTION_HANDLER_PANIC(30, "Security Exception");
EXCEPTION_HANDLER_PANIC(31, "Unknown (R)");

static void unimplemented_interrupt()
{
	k_printf("Unimplemented Interrupt");
}

void set_interrupt_handlers()
{
	set_idt_entry(0x00, IDT_INTERRUPT_GATE_TA, exception_0);
	set_idt_entry(0x01, IDT_INTERRUPT_GATE_TA, exception_1);
	set_idt_entry(0x02, IDT_INTERRUPT_GATE_TA, exception_2);
	set_idt_entry(0x03, IDT_TRAP_GATE_TA, exception_3);
	set_idt_entry(0x04, IDT_TRAP_GATE_TA, exception_4);
	set_idt_entry(0x05, IDT_INTERRUPT_GATE_TA, exception_5);
	set_idt_entry(0x06, IDT_INTERRUPT_GATE_TA, exception_6);
	set_idt_entry(0x07, IDT_INTERRUPT_GATE_TA, exception_7);
	set_idt_entry(0x08, IDT_TRAP_GATE_TA, exception_8);
	set_idt_entry(0x09, IDT_INTERRUPT_GATE_TA, exception_9);
	set_idt_entry(0x0a, IDT_INTERRUPT_GATE_TA, exception_10);
	set_idt_entry(0x0b, IDT_INTERRUPT_GATE_TA, exception_11);
	set_idt_entry(0x0c, IDT_INTERRUPT_GATE_TA, exception_12);
	set_idt_entry(0x0d, IDT_INTERRUPT_GATE_TA, exception_13);
	set_idt_entry(0x0e, IDT_INTERRUPT_GATE_TA, exception_14);
	set_idt_entry(0x0f, IDT_TRAP_GATE_TA, exception_15);
	set_idt_entry(0x10, IDT_INTERRUPT_GATE_TA, exception_16);
	set_idt_entry(0x11, IDT_INTERRUPT_GATE_TA, exception_17);
	set_idt_entry(0x12, IDT_TRAP_GATE_TA, exception_18);
	set_idt_entry(0x13, IDT_INTERRUPT_GATE_TA, exception_19);
	set_idt_entry(0x14, IDT_INTERRUPT_GATE_TA, exception_20);
	set_idt_entry(0x15, IDT_TRAP_GATE_TA, exception_21);
	set_idt_entry(0x16, IDT_TRAP_GATE_TA, exception_22);
	set_idt_entry(0x17, IDT_TRAP_GATE_TA, exception_23);
	set_idt_entry(0x18, IDT_TRAP_GATE_TA, exception_24);
	set_idt_entry(0x19, IDT_TRAP_GATE_TA, exception_25);
	set_idt_entry(0x1a, IDT_TRAP_GATE_TA, exception_26);
	set_idt_entry(0x1b, IDT_TRAP_GATE_TA, exception_27);
	set_idt_entry(0x1c, IDT_TRAP_GATE_TA, exception_28);
	set_idt_entry(0x1d, IDT_TRAP_GATE_TA, exception_29);
	set_idt_entry(0x1e, IDT_TRAP_GATE_TA, exception_30);
	set_idt_entry(0x1f, IDT_TRAP_GATE_TA, exception_31);

	// FIXME: Actually implement these properly
	for(uint16_t i = 0x20; i < 0xff; i++) {
		set_idt_entry(i, IDT_TRAP_GATE_TA, unimplemented_interrupt);
	}
}

}
