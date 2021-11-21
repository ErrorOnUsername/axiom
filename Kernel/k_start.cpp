#include <AXUtil/Assert.h>
#include <AXUtil/List.h>
#include <Kernel/Std.h>
#include <AXUtil/Types.h>
#include <Kernel/IO.h>
#include <Kernel/k_debug.h>
#include <Kernel/Arch/CPU.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>
#include <Kernel/Arch/x86_64/Interrupts/IDT.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>
#include <Kernel/Memory/MemoryManager.h>
#include <Kernel/Memory/Heap/k_malloc.h>

namespace Kernel {

extern "C" void k_start(stivale2_struct* stivale2_struct)
{
	auto* framebuffer_tag = (stivale2_struct_tag_framebuffer*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
	auto* memory_map_tag = (stivale2_struct_tag_memmap*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

	if (memory_map_tag == nullptr) {
		klog(LogModeError | LogModeBoot, "Couldn't find the stivale2 memory map tag :'(");
		for(;;);
	} else if(framebuffer_tag == nullptr) {
		klog(LogModeError | LogModeBoot, "Couldn't find the stivale2 framebuffer tag :'(");
		for(;;);
	}

	k_malloc_init();

	GDT::init_gdt();
	IDT::init_idt();

	// =====================================================
	// ==      Create Generic Bootloader Memory Map       ==
	// =====================================================

	uint64_t memory_map_entry_count = memory_map_tag->entries;
	stivale2_mmap_entry* memmap_entries = memory_map_tag->memmap;
	BootloaderMemoryMapEntry memory_map_entries[memory_map_entry_count];

	for(uint64_t i = 0; i < memory_map_entry_count; i++)
		memory_map_entries[i] = memory_map_entry_from_stivale2_entry(memmap_entries[i]);

	BootloaderMemoryMap memory_map = {
		.length = memory_map_entry_count,
		.entries = &memory_map_entries[0],
	};

	// =====================================================
	// ==            Initialize Memory Manager            ==
	// =====================================================

	if(Memory::MemoryManager::initialize(memory_map) == KResult::Error) {
		klog(LogModeError | LogModeMemoryManager, "Failed to create memory manager.");
		CPU::halt();
	}

	// This looks ugly, but that's ok. It looks pretty when it gets printed.
	const char* banner = "__          ________ _      _____ ____  __  __ ______     _______ ____\n"
	                     "\\ \\        / /  ____| |    / ____/ __ \\|  \\/  |  ____|   |__   __/ __ \\\n"
	                     " \\ \\  /\\  / /| |__  | |   | |   | |  | | \\  / | |__         | | | |  | |\n"
	                     "  \\ \\/  \\/ / |  __| | |   | |   | |  | | |\\/| |  __|        | | | |  | |\n"
	                     "   \\  /\\  /  | |____| |___| |___| |__| | |  | | |____       | | | |__| /\n"
	                     "    \\/  \\/   |______|______\\_____\\____/|_|  |_|______|      |_|  \\____/\n"
	                     "\n"
	                     "          __   _______ ____  __  __\n"
	                     "    /\\    \\ \\ / /_   _/ __ \\|  \\/  |\n"
	                     "   /  \\    \\ V /  | || |  | | \\  / |\n"
	                     "  / /\\ \\    > <   | || |  | | |\\/| |\n"
	                     " / ____ \\  / . \\ _| || |__| | |  | |\n"
	                     "/_/    \\_\\/_/ \\_\\_____\\____/|_|  |_|\n";


	k_printf("%s\n", banner);

	uint32_t*      framebuffer_address = (uint32_t*)framebuffer_tag->framebuffer_addr;
	const uint16_t framebuffer_width   = framebuffer_tag->framebuffer_width;
	const uint16_t framebuffer_height  = framebuffer_tag->framebuffer_height;

	// Really disgusting pallete test
	uint32_t tick = 0;
	for(;;) {
		for(uint32_t i = 0; i < framebuffer_height * framebuffer_width; i++) {
			uint8_t  i_color   = (i + tick) % 0xff;
			uint8_t  x_color   = ((i + tick) % framebuffer_width) % 0xff;
			uint8_t  y_color   = ((i + tick) / framebuffer_width) % 0xff;
			uint32_t out_color = ((uint32_t)x_color << 16) | ((uint32_t)y_color << 8) | ((uint32_t)i_color);

			framebuffer_address[i] = out_color;
			tick = (tick % 0xffffffff) + 1;
		}
	}
}

}

// Just some C++ ABI garbage to make the linker shut up.
void* __dso_handle __attribute__((visibility("hidden")));
