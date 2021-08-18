#include "MemoryManager.h"

#include <Kernel/k_stdio.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>

namespace Kernel {

void MemoryManager::init(stivale2_struct_tag_memmap* memmap_tag)
{
	auto* memmap = memmap_tag->memmap;
	uint64_t memmap_entries = memmap_tag->entries;

	k_printf("The bootloader detected the following memory map:\n");
	for(uint64_t i = 0; i < memmap_entries; i++) {
		auto const& current_entry = memmap[i];
		k_printf("base: %xl, length: %xl, type: %s\n", current_entry.base, current_entry.length, memmap_type_as_string(current_entry.type));
	}
}

MemoryManager::MemoryManager()
{ }

MemoryManager::~MemoryManager()
{ }

}
