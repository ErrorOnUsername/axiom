#pragma once

#include <Kernel/Arch/x86_64/Boot/stivale2.h>

namespace Kernel {

class MemoryManager {
public:
	static void init(stivale2_struct_tag_memmap*);

	MemoryManager& instance();

	void setup_paging();

private:
	MemoryManager();
	~MemoryManager();
};

}
