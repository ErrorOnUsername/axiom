#pragma once

namespace Kernel {

struct Process {
	int parent_pid;
	int pid;

	int uid;
	int gid;
};

};
