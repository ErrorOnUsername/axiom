#include <AXUtil/Types.h>
#include <Kernel/IO.h>
#include <Kernel/k_stdio.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>

namespace Kernel {

extern "C" void k_start(stivale2_struct* stivale2_struct)
{
	init_custom_gdt();

	auto* framebuffer_tag = (stivale2_struct_tag_framebuffer*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
	auto* memmap_tag = (stivale2_struct_tag_memmap*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

	if (memmap_tag == nullptr) {
		k_printf("Couldn't find the memmap tag :'(\n");
		for(;;) { }
	} else if(framebuffer_tag == nullptr) {
		k_printf("Couldn't find the framebuffer tag :'(\n");
		for(;;) { }
	}

	k_printf("Welcome to Axiom! :)\n");

	auto* memmap = memmap_tag->memmap;
	uint64_t memmap_entries = memmap_tag->entries;

	k_printf("The bootloader detected the following memory map:\n");
	for(uint64_t i = 0; i < memmap_entries; i++) {
		auto const& current_entry = memmap[i];
		k_printf("base: %xl, length: %xl, type: %s\n", current_entry.base, current_entry.length, memmap_type_as_string(current_entry.type));
	}

	for(;;);
}

}
