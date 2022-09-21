#include <libs/ax/result.hh>
#include <libs/ax/types.hh>
#include <kernel/k_debug.hh>
#include <kernel/arch/init.hh>
#include <kernel/arch/processor.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/memory_manager.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/region.hh>
#include <kernel/memory/heap/k_malloc.hh>
#include <kernel/system/early_console/early_console.hh>
#include <kernel/system/scheduler/scheduler.hh>

namespace Kernel {

// This looks ugly, but that's ok. It looks pretty when it gets printed.
const char* banner = "\t__          ________ _      _____ ____  __  __ ______     _______ ____\n"
                     "\t\\ \\        / /  ____| |    / ____/ __ \\|  \\/  |  ____|   |__   __/ __ \\\n"
                     "\t \\ \\  /\\  / /| |__  | |   | |   | |  | | \\  / | |__         | | | |  | |\n"
                     "\t  \\ \\/  \\/ / |  __| | |   | |   | |  | | |\\/| |  __|        | | | |  | |\n"
                     "\t   \\  /\\  /  | |____| |___| |___| |__| | |  | | |____       | | | |__| /\n"
                     "\t    \\/  \\/   |______|______\\_____\\____/|_|  |_|______|      |_|  \\____/\n"
                     "\n"
                     "\t          __   _______ ____  __  __\n"
                     "\t    /\\    \\ \\ / /_   _/ __ \\|  \\/  |\n"
                     "\t   /  \\    \\ V /  | || |  | | \\  / |\n"
                     "\t  / /\\ \\    > <   | || |  | | |\\/| |\n"
                     "\t / ____ \\  / . \\ _| || |__| | |  | |\n"
                     "\t/_/    \\_\\/_/ \\_\\_____\\____/|_|  |_|\n";

inline static Processor& boot_processor()
{
	alignas(Processor) static u8 boot_proc_storage[sizeof(Processor)];
	return (Processor&)boot_proc_storage;
}

extern "C" void on_task_switch_timer();

extern "C" void k_init(Memory::BootloaderMemoryMap& memory_map, addr_t framebuffer_addr, u16 framebuffer_width, u16 framebuffer_height)
{
	k_malloc_init();

	new (&boot_processor()) Processor();
	boot_processor().initialize(0);

	Memory::init_memory_management(memory_map);

	k_printf("[33;1mStarting up Axiom...[0m\n");

	Arch::init();
	Scheduler::init();

	k_printf("%s\n", banner);

	EarlyConsole::init((u32*)framebuffer_addr, framebuffer_width, framebuffer_height);

	for(size_t i = 0; banner[i] != 0; i++)
		EarlyConsole::put_char(banner[i]);

	for(;;);
}

}

// Just some C++ ABI garbage to make the linker shut up.
void* __dso_handle __attribute__((visibility("hidden")));
