#include <stdint.h>
#include <stddef.h>

#include <AXUtil/Types.hh>
#include <Kernel/bootloader.hh>

extern "C" void k_start(stivale2_struct* stivale2_struct)
{
	auto* terminal_tag = (stivale2_struct_tag_terminal*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

	if(terminal_tag == nullptr)
		for(;;) { }

	void* term_write_ptr = (void*)terminal_tag->term_write;
	void (*term_write) (const char* str, size_t length) = (void (*) (const char*, size_t))term_write_ptr;
	auto* memmap = (stivale2_struct_tag_memmap*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

	// TODO: MAKE AN ACTUAL PRINT FUNCTION!
	term_write("Welcome to Axiom! :)", 20);

	for(;;) { }
}
