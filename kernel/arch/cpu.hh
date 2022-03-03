#pragma once

namespace Kernel {

struct CPU {
	static void halt();
	static void halt_all();
	static void disable_interrupts();
	static void enable_interrupts();
};

};
