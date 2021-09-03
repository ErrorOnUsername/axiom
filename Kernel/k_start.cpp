#include <AXUtil/Types.h>
#include <Kernel/IO.h>
#include <Kernel/k_stdio.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>
#include <Kernel/Arch/x86_64/Interrupts/IDT.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>
#include <Kernel/Memory/MemoryManager.h>

namespace Kernel {

extern "C" void k_start(stivale2_struct* stivale2_struct)
{
	init_gdt();
	init_idt();

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


	// FIXME: Something here is broken. I need to fix it, but I'll come back to this later.
	asm volatile("int $0x0e"); // Software-triggered page fault

	MemoryManager::init(memmap_tag);

	auto* framebuffer_address = (uint32_t*)framebuffer_tag->framebuffer_addr;
	const uint16_t framebuffer_width = framebuffer_tag->framebuffer_width;
	const uint16_t framebuffer_height = framebuffer_tag->framebuffer_height;

	// Really disgusting pallete test
	uint32_t tick = 0;
	for(;;) {
		for(uint32_t i = 0; i < framebuffer_height * framebuffer_width; i++) {
			uint8_t i_color = (i + tick) % 0xff;
			uint8_t x_color = ((i + tick) % framebuffer_width) % 0xff;
			uint8_t y_color = ((i + tick) / framebuffer_width) % 0xff;
			uint32_t out_color = ((uint32_t)i_color << 16) | ((uint32_t)y_color << 8) | ((uint32_t)x_color);
			framebuffer_address[i] = out_color;
			tick = (tick % 0xffffffff) + 1;
		}
	}
}

}
