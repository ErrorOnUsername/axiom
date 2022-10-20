#pragma once

#include <libs/ax/types.hh>
#include <kernel/system/scheduler/thread.hh>

namespace Kernel {

// This is entirely arbitrary. Maybe we can be more intelligent about this?
// idk. 32 cores sounds like more than enough. I say that, but people used to
// think we'd never need more than 64K ram. and look how that turned out lol
constexpr u8 MAX_PROCESSOR_COUNT = 32;

struct Processor {
	Thread* idle_thread;
	Thread* thread;

	u8 cpu_id;

	void set_current_thread();

	static Thread* current_thread();
	static Thread* current_idle_thread();

	void initialize(u8 cpu);
};

}
