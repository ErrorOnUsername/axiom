#pragma once

#include <ax_util/helpers.hh>

namespace Kernel {

struct ScopeInterruptDisabler {
	MAKE_NONCOPYABLE(ScopeInterruptDisabler);
	MAKE_NONMOVEABLE(ScopeInterruptDisabler);

	ScopeInterruptDisabler()  { asm("cli;"); }
	~ScopeInterruptDisabler() { asm("sti;"); }
};

}
