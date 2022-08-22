#include "panic.hh"

#include <kernel/arch/cpu.hh>
#include <kernel/system/early_console/early_console.hh>
#include <kernel/arch/interrupts.hh>

namespace Kernel {

// FIXME: Supply more information when we panic. For instance:
//          * Stack trace
//          * Register states
void panic(char const* msg)
{
	ScopeInterruptDisabler interrupt_disabler;

	for(size_t i = 0; msg[i] != 0; i++)
		EarlyConsole::put_char(msg[i]);

	CPU::halt();
}

}
