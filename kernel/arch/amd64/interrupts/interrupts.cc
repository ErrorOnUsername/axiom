#include "interrupts.hh"

#include <libs/ax/helpers.hh>
#include <libs/ax/types.hh>
#include <kernel/k_debug.hh>
#include <kernel/arch/amd64/boot/tables.hh>
#include <kernel/arch/amd64/register_state.hh>
#include <kernel/arch/amd64/interrupts/pic.hh>

namespace Kernel {

static constexpr u8 IRQ_BASE = 32;
static void* irq_handlers[16];

char const* exception_code_names[32] = {
	"Divide-By-Zero Error",
	"Debug",
	"NMI",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invlaid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	"Reserved"
};

extern "C" void common_interrupt_handler(RegisterState*);
void common_interrupt_handler(RegisterState* registers)
{
	if(registers->interrupt_vector < 32) {
		klog(LogLevel::Error, "EXCEPTION TRIGGERED!! Dumping State...");

		k_printf("\nException Type: %s\n", exception_code_names[registers->interrupt_vector]);

		// TODO: Parse the error code so that we get more useful information
		k_printf("\nError Code: %xl\n", registers->error_code);

		k_printf("RIP: %xl\n", registers->rip);

		k_printf("RSP: %xl, RBP: %xl\n", registers->rsp, registers->rbp);
		k_printf("RSI: %xl, RDI: %xl\n", registers->rsi, registers->rdi);

		k_printf("RAX: %xl, RBX: %xl\n", registers->rax, registers->rbx);
		k_printf("RCX: %xl, RDX: %xl\n", registers->rcx, registers->rdx);

		k_printf("R8:  %xl, R9:  %xl\n", registers->r8, registers->r9);
		k_printf("R10: %xl, R11: %xl\n", registers->r10, registers->r11);
		k_printf("R12: %xl, R13: %xl\n", registers->r12, registers->r13);
		k_printf("R14: %xl, R15: %xl\n", registers->r14, registers->r15);

		asm volatile("cli; hlt");
		for(;;); // should never be reached
	}

	int irq = registers->interrupt_vector - IRQ_BASE;

	auto handler = (IRQHandler)irq_handlers[irq];

	if(handler)
		handler(*registers);
	else
		klogf(LogLevel::Warning, "The irq %d does not have a handler registered!", (u8)(registers->interrupt_vector - IRQ_BASE));

	PIC::end_of_interrupt(irq);
}

void register_irq(u8 irq_number, IRQHandler handler)
{
	irq_handlers[irq_number] = (void*)handler;
}

}
