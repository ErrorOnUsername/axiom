#pragma once

#include <libs/ax/result.hh>
#include <kernel/system/scheduler/proccess.hh>
#include <kernel/system/scheduler/thread.hh>

namespace Kernel {

namespace Scheduler {

void init();

void enqueue_thread(Thread*);

}

}
