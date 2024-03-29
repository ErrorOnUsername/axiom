#include "boot_stivale.hh"

#include <libs/ax/helpers.hh>
#include <libs/ax/types.hh>

#include <kernel/k_debug.hh>
#include <kernel/arch/amd64/boot/stivale2.h>
#include <kernel/arch/amd64/interrupts/pic.hh>

namespace Kernel {

static u8 k_stack[32768];

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

void* get_stivale2_tag(stivale2_struct* stivale2_struct, u64 id)
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

extern "C" void k_init(Memory::BootloaderMemoryMap&, addr_t framebuffer_addr, u16 framebuffer_width, u16 framebuffer_height);

extern "C" void boot_entry(stivale2_struct* stivale2_struct)
{
	auto* framebuffer_tag = (stivale2_struct_tag_framebuffer*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
	auto* memory_map_tag  = (stivale2_struct_tag_memmap*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

	if (memory_map_tag == nullptr) {
		klog(LogLevel::Error, "Couldn't find the stivale2 memory map tag :'(");
		for(;;);
	} else if(framebuffer_tag == nullptr) {
		klog(LogLevel::Error, "Couldn't find the stivale2 framebuffer tag :'(");
		for(;;);
	}

	u64                  memory_map_entry_count = memory_map_tag->entries;
	stivale2_mmap_entry* memmap_entries         = memory_map_tag->memmap;

	Memory::BootloaderMemoryMapEntry memory_map_entries[memory_map_entry_count];

	for(uint64_t i = 0; i < memory_map_entry_count; i++) {
		memory_map_entries[i] = {
			.address = memmap_entries[i].base,
			.size    = memmap_entries[i].length,
			.type    = memmap_entries[i].type
		};
	}

	Memory::BootloaderMemoryMap memory_map = {
		.entries = &memory_map_entries[0],
		.length = memory_map_entry_count
	};

	//
	// Arch-specific init
	//

	k_init(memory_map, framebuffer_tag->framebuffer_addr, framebuffer_tag->framebuffer_width, framebuffer_tag->framebuffer_height);
}

}
