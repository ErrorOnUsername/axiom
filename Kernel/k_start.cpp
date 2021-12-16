#include <AXUtil/Assert.h>
#include <AXUtil/List.h>
#include <AXUtil/Types.h>
#include <Kernel/Std.h>
#include <Kernel/IO.h>
#include <Kernel/k_debug.h>
#include <Kernel/Arch/CPU.h>
#include <Kernel/Arch/x86_64/Boot/boot_stivale.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>
#include <Kernel/Arch/x86_64/Interrupts/IDT.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>
#include <Kernel/Memory/MemoryManager.h>
#include <Kernel/Memory/Heap/k_malloc.h>

namespace Kernel {

extern "C" void k_init(Memory::BootloaderMemoryMap& memory_map)
{
	k_malloc_init();

	GDT::init_gdt();
	IDT::init_idt();

	Memory::MemoryManager::initialize(memory_map);

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
