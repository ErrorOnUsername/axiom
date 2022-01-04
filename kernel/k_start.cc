#include <ax_util/types.hh>
#include <kernel/k_debug.hh>
#include <kernel/arch/rtc.hh>
#include <kernel/arch/amd64/gdt/gdt.hh>
#include <kernel/arch/amd64/interrupts/idt.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/memory_manager.hh>
#include <kernel/memory/heap/k_malloc.hh>

namespace Kernel {

extern "C" void k_init(Memory::BootloaderMemoryMap& memory_map)
{
	k_malloc_init();

	GDT::init_gdt();
	IDT::init_idt();

	Memory::init_memory_management(memory_map);

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

	// uint32_t*      framebuffer_address = (uint32_t*)framebuffer_tag->framebuffer_addr;
	// const uint16_t framebuffer_width   = framebuffer_tag->framebuffer_width;
	// const uint16_t framebuffer_height  = framebuffer_tag->framebuffer_height;

	for(;;);
}

}

// Just some C++ ABI garbage to make the linker shut up.
void* __dso_handle __attribute__((visibility("hidden")));
