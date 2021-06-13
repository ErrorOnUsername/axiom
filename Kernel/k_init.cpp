#include <AXUtil/types.h>
#include <Kernel/Graphics/VGATextModeDevice.h>

#define VGA_TXT_BFR_ADDR 0xB8000
#define VGA_TXT_LIGHT_GRAY_ATTRIB 7

#define COLUMNS 80
#define ROWS 25

extern "C" void k_init();

extern "C" void k_init() {
	// Hey, idiot don't forget to pass the multiboot_info ptr.
	//
	// Also, we need to take the magic value the bootloader gives
	// us and make sure that it matches what it's supposed to. It's
	// not suuuuuper important, but it's in the spec, so we might
	// as well slap it in here. :)

	VGATextModeDevice vga_dev;

	vga_dev.clear_screen();
	vga_dev.write_line("Welcome to Axiom!");

	while(1);
}
