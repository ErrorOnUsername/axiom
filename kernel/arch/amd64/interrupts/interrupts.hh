#pragma once

#include <libs/ax/helpers.hh>
#include <kernel/arch/amd64/register_state.hh>

namespace Kernel {

struct ScopeInterruptDisabler {
	MAKE_NONCOPYABLE(ScopeInterruptDisabler);
	MAKE_NONMOVEABLE(ScopeInterruptDisabler);

	ScopeInterruptDisabler()  { asm("cli;"); }
	~ScopeInterruptDisabler() { asm("sti;"); }
};

using IRQHandler = void(*)(RegisterState&);

void register_irq(u8 irq_number, IRQHandler);

}
