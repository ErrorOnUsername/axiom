#include "boot_stivale.h"

#include <AXUtil/Helpers.h>
#include <AXUtil/Types.h>
#include <Kernel/k_debug.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>

namespace Kernel {

static uint8_t k_stack[32768];

static stivale2_header_tag_framebuffer framebuffer_header_tag {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = 0,
	},
	.framebuffer_width = 1280,
	.framebuffer_height = 1024,
	.framebuffer_bpp = 32,
};

SECTION(.stivale2hdr)
static stivale2_header stivale2_header {
	.entry_point = 0,
	.stack = (uintptr_t)k_stack + sizeof(k_stack),
	.flags = (1 << 1),
	.tags = (uintptr_t)&framebuffer_header_tag
};

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id)
{

	auto* current_tag = (stivale2_tag*)stivale2_struct->tags;
	for(;;) {
		if(current_tag == nullptr) {
			return nullptr;
		}
		if(current_tag->identifier == id) {
			return current_tag;
		}

		current_tag = (stivale2_tag*)current_tag->next;
	}
}


Memory::BootloaderMemoryMapEntry memory_map_entry_from_stivale2_entry(stivale2_mmap_entry const& entry)
{
	return {
		.address = entry.base,
		.size = entry.length,
		.type = entry.type
	};
}

extern "C" void k_init(Memory::BootloaderMemoryMap&);

extern "C" void boot_entry(stivale2_struct* stivale2_struct)
{
	auto* framebuffer_tag = (stivale2_struct_tag_framebuffer*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
	auto* memory_map_tag = (stivale2_struct_tag_memmap*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

	if (memory_map_tag == nullptr) {
		klog(LogLevel::Error, "Couldn't find the stivale2 memory map tag :'(");
		for(;;);
	} else if(framebuffer_tag == nullptr) {
		klog(LogLevel::Error, "Couldn't find the stivale2 framebuffer tag :'(");
		for(;;);
	}

	uint64_t memory_map_entry_count = memory_map_tag->entries;
	stivale2_mmap_entry* memmap_entries = memory_map_tag->memmap;
	Memory::BootloaderMemoryMapEntry memory_map_entries[memory_map_entry_count];

	for(uint64_t i = 0; i < memory_map_entry_count; i++)
		memory_map_entries[i] = memory_map_entry_from_stivale2_entry(memmap_entries[i]);

	Memory::BootloaderMemoryMap memory_map = {
		.length = memory_map_entry_count,
		.entries = &memory_map_entries[0],
	};

	k_init(memory_map);
}

}
