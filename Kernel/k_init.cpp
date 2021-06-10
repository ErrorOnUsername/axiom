#include <AXUtil/types.h>

#define VGA_TXT_BFR_ADDR 0xB8000
#define VGA_TXT_WHITE_ATTRIB 7

extern "C" void k_init();
extern "C" void k_println(const char*);

extern "C" void k_init() {
	// Hey, idiot don't forget to pass the multiboot_info ptr.
	//
	// Also, we need to take the magic value the bootloader gives
	// us and make sure that it matches what it's supposed to. It's
	// not suuuuuper important, but it's in the spec, so we might
	// as well slap it in here. :)

	k_println("Hello, World!");
	for(;;) {
		asm("hlt");
	}
}

extern "C" void k_println(const char* msg) {
	// TODO: Actually implement a print function.
	char c = msg[0];
	int idx = 0;
	while(c != '\0') {
		// TODO: Actually put a character in the VGA text buffer.
		c = msg[++idx];
	}
	// NEWLINE
}
