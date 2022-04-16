#pragma once

#include <ax_util/types.hh>
#include <kernel/system/scheduler/thread.hh>

namespace Kernel {

// This is entirely arbitrary. Maybe we can be more intelligent about this?
// idk. 32 cores sounds like more than enough. I say that, but people used to
// think we'd never need more than 64K ram. and look how that turned out lol
constexpr size_t MAX_PROCESSOR_COUNT = 32;

struct Proccessor {
	static void    initialize();
	static Thread* current_thread();
};

}
