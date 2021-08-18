#include <AXUtil/Types.h>
#include <Kernel/IO.h>
#include <Kernel/k_stdio.h>
#include <Kernel/Arch/x86_64/Boot/boot.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>
#include <Kernel/Arch/x86_64/GDT/GDT.h>
#include <Kernel/Memory/MemoryManager.h>

#define W 0x00ff00ff
#define B 0x00000000

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

	MemoryManager::init(memmap_tag);

	/* WE MUST DISPLAY E */
	auto* framebuffer_address = (uint32_t*)framebuffer_tag->framebuffer_addr;

	const uint16_t framebuffer_width = framebuffer_tag->framebuffer_width;
	const uint16_t framebuffer_height = framebuffer_tag->framebuffer_height;

	/* Literally just a bitmap for the letter E */
	uint32_t E[] =
	{
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, B, B, B, B, B, B, B, B, B, B, B, B,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
		W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W,
	};

	constexpr uint16_t start_x = 100;
	constexpr uint16_t end_x = 117;

	constexpr uint16_t start_y = 100;
	constexpr uint16_t end_y = 139;

	for(uint16_t y = start_y; y < end_y; y++) { for(uint16_t x = start_x; x < end_x; x++) {
			framebuffer_address[x + (y * framebuffer_width)] = E[(x-start_x) + ((y-start_y) * 18)];
		}
	}


	for(;;);
}

}
