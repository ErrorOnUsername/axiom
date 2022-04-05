#pragma once

#include <kernel/memory/paging.hh>

namespace Kernel {

struct Process {
	Memory::AddressSpace* addr_space;

	int parent_pid;
	int pid;
	int uid;
	int gid;

	void enter_context();
};

};
